//============================================================================
// Name        : Ravioli Racing
// Author      : Branden Lee
// Version     : 0.01
// Copyright   : GPL
// Description : C++ Racing Game
//
// IDE: Eclipse Version: Neon.2 Release (4.6.2)
// Requires Cygwin in windows and GCC in linux
//============================================================================
#include "../src/WIN_SOCKET.h"
#ifdef _WIN32

void WIN_SOCKET::setPort(int port_, unsigned long IP_address) {
	port = (u_short) port_;
	//setup a socket and connection tools
	memset((char*) &sockAddr, 0, sizeof(sockAddr));
	sockFamily = "AF_INET";
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = IP_address;
	sockAddr.sin_port = htons(port);
	listenFlag = false;
}

void WIN_SOCKET::setHost(std::string host_) {
	host = host_;
	char* hostC = new char[host.length() + 1];
	strcpy(hostC, host.c_str());

	/* get IP by host */
	std::string errMsg;
	struct addrinfo hints, *res;
	int err;

	WSAData data;
	WSAStartup(MAKEWORD(2, 0), &data);

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;
	hints.ai_protocol = 0;

	if ((err = getaddrinfo(host_.c_str(), NULL, &hints, &res)) != 0) {
		errMsg = "";
		if (err == 11001) {
			errMsg = "Host not found";
		}
		throw std::runtime_error(
				"Error Code " + std::to_string(err)
				+ (errMsg.length() > 0 ? ": " + errMsg : ""));
	}
	try {
		sockAddrUn =
		((struct sockaddr_in *) (res->ai_addr))->sin_addr.S_un.S_addr;
	} catch (const std::exception& e) {
		throw std::runtime_error(
				"Could not connect to the internet." + std::string(e.what()));
	}
	freeaddrinfo(res);
	WSACleanup();
}

WIN_SOCKET::WIN_SOCKET() {
	rcvMsgCurrent = "";
	sendMsgCurrent = "";
	connectionLastIndex = -1;
	connectFlag = true;
}

void WIN_SOCKET::sockSetup() {
	// Initialize Winsock
	int iResult;
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		throw std::runtime_error("WSAStartup failed: " + iResult);
	}
	/* open stream oriented socket with internet address
	 * also keep track of the socket descriptor
	 * */
	sockId = socket(AF_INET, SOCK_STREAM, 0);
	if (sockId < 0) {
		throw std::runtime_error("Error establishing the server socket");
	}
}

void WIN_SOCKET::sockClientConnect() {
	//try to connect...
	int status = connect(sockId, (sockaddr*) &sockAddr, sizeof(sockAddr));
	if (status != 0) {
		throw std::runtime_error("Error connecting to socket!");
	}
	connectFlag = true;
}

void WIN_SOCKET::sockClientLoop() {
	std::chrono::system_clock::time_point t1, t2;
	unsigned int elapsedTime = 0;
	t1 = std::chrono::system_clock::now();
	if (CBmap.find("connect") != CBmap.end())
	CBmap["connect"](this);
	while (1) {
		std::cout << "[" << getSockId() << "] SELECT" << std::endl;
		if (cmdRCV() <= 0) {
			std::cout << "[" << getSockId() << "] CLOSING: read 0 bytes"
			<< std::endl;
			break;
		}
	}
	sockClose();
	t2 = std::chrono::system_clock::now();
	elapsedTime = (int) std::chrono::duration_cast<std::chrono::seconds>(
			t2 - t1).count();
	std::cout << "********Session********" << std::endl;
	std::cout << "Bytes written: " << sendBytes << " Bytes read: " << rcvBytes
	<< std::endl;
	std::cout << "Elapsed time: " << elapsedTime << " secs" << std::endl;
	std::cout << "Connection closed..." << std::endl;
}

void WIN_SOCKET::sockSrvBind() {
	//bind the socket to its local address
	int bindStatus = bind(sockId, (struct sockaddr*) &sockAddr,
			sizeof(sockAddr));
	if (bindStatus < 0) {
		throw std::runtime_error("Error binding socket to local address");
	}
	connectFlag = true;
}

void WIN_SOCKET::sockSrvListen() {
	//listen for up to 5 requests at a time
	listen(sockId, 5);
	listenFlag = true;
	//lets keep track of the session time
	std::chrono::system_clock::time_point t1, t2;
	unsigned int elapsedTime = 0;
	t1 = std::chrono::system_clock::now();
	//also keep track of the amount of data sent as well
	int nfds = sockId;
	int i, n, activity;
	if (CBmap.find("listening") != CBmap.end())
	CBmap["listening"](this);
	while (listenFlag) {
		std::cout << "[" << getSockId() << "] SELECT" << std::endl;
		build_fd_sets(&read_fds, &write_fds, &except_fds);
		/* linux only: nfds should be set to the highest-numbered file descriptor in any of the three sets, plus 1. */
		nfds = sockId;
		n = connectionList.size();
		for (i = 0; i < n; ++i) {
			if (connectionList[i]->getSockId() > nfds)
			nfds = connectionList[i]->getSockId();
		}

		activity = select(nfds + 1, &read_fds, &write_fds, &except_fds,
				NULL);
		if (activity == 0) {
			if (CBmap.find("error") != CBmap.end())
			CBmap["error"](this);
		} else if (activity == -1) {
			if (CBmap.find("error") != CBmap.end())
			CBmap["error"](this);
		}

		sockCon();
		sockDcon();

		if (FD_ISSET(sockId, &except_fds)) {
			if (CBmap.find("error") != CBmap.end())
			CBmap["error"](this);
		}

		n = connectionList.size();
		for (i = 0; i < n; ++i) {
			if (connectionList[i]->activeCon()) {
				if (FD_ISSET(connectionList[i]->getSockId(), &read_fds)) {
					if (connectionList[i]->cmdRCV() <= 0) {
						std::cout << "[" << connectionList[i]->getSockId()
						<< "] CLOSING: read 0 bytes" << std::endl;
						connectionList[i]->sockClose();
					}
				}
				if (FD_ISSET(connectionList[i]->getSockId(), &write_fds)) {
					connectionList[i]->sockSend();
				}

				if (FD_ISSET(connectionList[i]->getSockId(), &except_fds)) {
					connectionList[i]->sockClose();
				}
			}
		}
	}
	t2 = std::chrono::system_clock::now();
	elapsedTime = (int) std::chrono::duration_cast<std::chrono::seconds>(
			t2 - t1).count();
	sockClose();
	std::cout << "********Session********" << std::endl;
	std::cout << "Bytes written: " << sendBytes << " Bytes read: " << rcvBytes
	<< std::endl;
	std::cout << "Elapsed time: " << elapsedTime << " secs" << std::endl;
	std::cout << "Connection closed..." << std::endl;
}

int WIN_SOCKET::sockCon() {
	int new_client_sock = 0;
	if (FD_ISSET(sockId, &read_fds)) {
		sockaddr_in client_addr;
		memset(&client_addr, 0, sizeof(client_addr));
		socklen_t client_len = sizeof(client_addr);
		new_client_sock = accept(sockId, (struct sockaddr *) &client_addr,
				&client_len);
		if (new_client_sock < 0) {
			throw std::runtime_error("Accept socket problem");
		}
		/* get ip address string */
		std::string hostname, service;
		/* server's client socket */
		WIN_SOCKET* socketClient = new WIN_SOCKET();
		socketClient->setSockId(new_client_sock);
		socketClient->setSockAddr(client_addr);
		connectionLastIndex = connectionList.size();
		connectionList.push_back(socketClient);
		if (CBmap.find("connection") != CBmap.end())
		CBmap["connection"](this);
		socketClient->emit("connect");
		socketClient->getHostName(hostname, service);
		std::cout << "[" << getSockId() << "] HOST = " << hostname
		<< " SERVICE = " << service << std::endl;
	}
	return new_client_sock;
}

int WIN_SOCKET::sockDcon() {
	int i;
	int n = connectionList.size();
	u_long iMode;
	int err = 0;
	for (i = 0; i < n; ++i) {
		if (FD_ISSET(connectionList[i]->getSockId(), &read_fds)) {
			iMode = 1;
			int iResult = ioctlsocket(connectionList[i]->getSockId(),
					FIONREAD, &iMode);
			if (iResult != NO_ERROR) {
				err++;
			}
			if (iMode == 0) {
				connectionList[i]->sockClose();
			}
		}
	}
	return err;
}

int WIN_SOCKET::cmdRCV() {
	int rcvByteCount = 0;
	std::string cmdStr;
	char temp[3] = {char(3), char(2), 0};
	std::string endSequence = std::string(temp);
	size_t endSequenceSize = endSequence.length();
	char msg[MAX_SEND_SIZE];
	memset(msg, '\0', MAX_SEND_SIZE); // fill with nulls to prevent garbage
	rcvByteCount += recv(sockId, msg, MAX_SEND_SIZE - 1, MSG_DONTWAIT);
	rcvBytes += rcvByteCount;
	if (rcvByteCount <= 0) {
		// error or socket closed
		return rcvByteCount;
	}
	std::cout << "[" << sockId << "] RCV = " << msg << std::endl;
	rcvMsgCurrent.append(msg);
	std::size_t pos = rcvMsgCurrent.find(endSequence);
	while (pos != std::string::npos && rcvMsgCurrent.length() > 0) {
		// end of message
		cmdStr = rcvMsgCurrent.substr(0, pos);
		std::cout << "[" << sockId << "] BUFF = " << rcvMsgCurrent << std::endl;
		std::cout << "[" << sockId << "] CMD = " << cmdStr << std::endl;
		rcvMsgList.push_back(cmdStr);
		rcvMsgCurrent.erase(0, pos + endSequenceSize);
		pos = rcvMsgCurrent.find(endSequence);
		if (CBmap.find("data") != CBmap.end())
		CBmap["data"](this);
	}
	return rcvByteCount;
}

int WIN_SOCKET::sockSend() {
	int sendByteCount = 0;
	size_t remainingSize = sendMsgCurrent.length();
	size_t sendLen;
	char msg[MAX_SEND_SIZE];
	while (remainingSize > 0) {
		std::cout << "[" << sockId << "] SEND BUFF = " << sendMsgCurrent
		<< std::endl;
		if (remainingSize > MAX_SEND_SIZE) {
			sendLen = MAX_SEND_SIZE;
		} else {
			sendLen = remainingSize;
		}
		strcpy(msg, sendMsgCurrent.substr(0, sendLen).c_str());
		std::cout << "[" << sockId << "] SEND = " << msg << std::endl;
		sendByteCount += send(sockId, (char*) &msg, sendLen, 0);
		sendMsgCurrent.erase(0, sendLen);
		remainingSize = sendMsgCurrent.length();
	}
	sendBytes += sendByteCount;
	if (CBmap.find("drain") != CBmap.end())
	CBmap["drain"](this);
	return sendByteCount;
}

void WIN_SOCKET::sockSend(std::string data) {
	/* when called from client's socket, sends to server
	 * when called from a server's client socket writes to a client */
	sendMsgCurrent.append(data);
	char temp[3] = {char(3), char(2), 0};
	sendMsgCurrent.append(std::string(temp));
	sockSend();
}

void WIN_SOCKET::sockSend(std::string data, int client_) {
	int i;
	int n = connectionList.size();
	for (i = 0; i < n; ++i)
	if (connectionList[i]->getSockId() == client_)
	connectionList[i]->sockSend(data);
}

void WIN_SOCKET::sockSendAll(std::string data) {
	int i;
	int n = connectionList.size();
	for (i = 0; i < n; ++i)
	connectionList[i]->sockSend(data);
}

int WIN_SOCKET::build_fd_sets(fd_set *read_fds, fd_set *write_fds,
		fd_set *except_fds) {
	int i;
	int n = connectionList.size();

	FD_ZERO(read_fds);
	FD_SET(sockId, read_fds);
	for (i = 0; i < n; ++i)
	if (connectionList[i]->activeCon())
	FD_SET(connectionList[i]->getSockId(), read_fds);

	FD_ZERO(write_fds);
	for (i = 0; i < n; ++i)
	if (connectionList[i]->activeCon()
			&& connectionList[i]->getSendMsgCurrent().length() > 0)
	FD_SET(connectionList[i]->getSockId(), write_fds);

	FD_ZERO(except_fds);
	FD_SET(sockId, except_fds);
	for (i = 0; i < n; ++i)
	if (connectionList[i]->activeCon())
	FD_SET(connectionList[i]->getSockId(), except_fds);

	return 0;
}

void WIN_SOCKET::listening(int port_) {
	std::string hostname, service;
	setPort(port_, htonl(INADDR_ANY));
	getHostName(hostname, service);
	sockSetup();
	std::cout << "[" << getSockId() << "] HOST = " << hostname << " SERVICE = "
	<< service << std::endl;
	sockSrvBind();
	sockSrvListen();
}

void WIN_SOCKET::listening(int port_, std::string host_) {
	std::string hostname, service;
	setHost(host_);
	setPort(port_, sockAddrUn);
	getHostName(hostname, service);
	sockSetup();
	std::cout << "[" << getSockId() << "] HOST = " << hostname << " SERVICE = "
	<< service << std::endl;
	sockSrvBind();
	sockSrvListen();
}

void WIN_SOCKET::connecting(int port_, std::string host_) {
	std::string hostname, service;
	setHost(host_);
	setPort(port_, sockAddrUn);
	getHostName(hostname, service);
	sockSetup();
	std::cout << "[" << getSockId() << "] HOST = " << hostname << " SERVICE = "
	<< service << std::endl;
	sockClientConnect();
	sockClientLoop();
}

void WIN_SOCKET::sockClose() {
	int i;
	int n = connectionList.size();
	std::cout << "[" << getSockId() << "] CLOSING" << std::endl;
	closesocket(sockId);
	connectFlag = false;
	for (i = 0; i < n; ++i)
	if (connectionList[i]->activeCon())
	connectionList[i]->sockClose();
	if (CBmap.find("close") != CBmap.end())
	CBmap["close"](this);
}

void WIN_SOCKET::on(std::string event, std::function<void(WIN_SOCKET*)> CB_) {
	CBmap[event] = CB_;
}

void WIN_SOCKET::emit(std::string event) {
	if (CBmap.find(event) != CBmap.end())
	CBmap[event](this);
}

int WIN_SOCKET::getPort() {
	return (int) port;
}
int WIN_SOCKET::getSockId() {
	return sockId;
}
std::string WIN_SOCKET::getFamily() {
	return sockFamily;
}
std::string WIN_SOCKET::getAddress() {
	return host;
}
WIN_SOCKET* WIN_SOCKET::clientLast() {
	if (connectionLastIndex >= 0) {
		return connectionList[connectionLastIndex];
	} else {
		throw std::runtime_error("No messages received yet");
	}
}

std::string WIN_SOCKET::rcvMsgLast() {
	int rcvMsgIndex = rcvMsgList.size() - 1;
	if (rcvMsgIndex >= 0) {
		return rcvMsgList[rcvMsgIndex];
	} else {
		throw std::runtime_error("No messages received yet");
	}
}

std::string WIN_SOCKET::errorMsgLast() {
	int errorMsgIndex = errorMsgList.size() - 1;
	if (errorMsgIndex >= 0) {
		return errorMsgList[errorMsgIndex];
	} else {
		throw std::runtime_error("No errors yet");
	}
}

std::string WIN_SOCKET::getSendMsgCurrent() {
	return sendMsgCurrent;
}

void WIN_SOCKET::setSockId(int sockId_) {
	sockId = sockId_;
}
void WIN_SOCKET::setSockAddr(sockaddr_in addr_) {
	sockAddr = addr_;
}

void WIN_SOCKET::getHostName(std::string &hostname_, std::string &service_) {
	char hostname[NI_MAXHOST];
	char servInfo[NI_MAXSERV];
	// Initialize Winsock
	int iResult;
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		throw std::runtime_error("WSAStartup failed: " + iResult);
	}
	/* get ip address string */
	DWORD dwRetval;
	dwRetval = getnameinfo((struct sockaddr *) &sockAddr,
			sizeof(struct sockaddr), hostname,
			NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);
	if (dwRetval == 0) {
		hostname_ = std::string(hostname);
		service_ = std::string(servInfo);
	} else {
		throw std::runtime_error("getnameinfo failed: " + dwRetval);
	}
	WSACleanup();
}

bool WIN_SOCKET::activeCon() {
	return connectFlag;
}

void WIN_SOCKET::commandSrvStr(std::string str) {
	std::string cmdDivider = "||";
	std::string argDivider = "##";
	size_t cmdDividerSize = cmdDivider.length();
	std::size_t pos = str.find(cmdDivider);
	if (pos != std::string::npos) {
		std::string cmd = str.substr(0, pos);
		std::string data = str.substr(pos + cmdDividerSize);
		std::vector<std::string>* argList = StringHelper::split(data,
				argDivider);
		std::cout << "[" << getSockId() << "] CMD = " << cmd << " DATA = "
		<< data << std::endl;
		if (cmd == "sendc") {
			// send client
			sockSend((*argList)[1], std::stoi((*argList)[0]));
		} else if (cmd == "sends") {
			// send server
			sockSend((*argList)[0]);
		} else if (cmd == "senda") {
			// broadcast
			sockSendAll((*argList)[0]);
		}
	}
}

void WIN_SOCKET::commandClientStr(std::string str) {
	commandSrvStr(str);
}
#endif
