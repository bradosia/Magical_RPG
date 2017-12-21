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
#include "WIN_SOCKET.h"
#ifdef _WIN32

WIN_SOCKET::WIN_SOCKET(int port_)
{
	port = (u_short) port_;
	//setup a socket and connection tools
	memset((char*) &servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);
	listenFlag = false;
}

WIN_SOCKET::WIN_SOCKET(std::string host_, int port_)
{
	port = (u_short) port_;
	host = host_;
	char* hostC = new char[host.length() + 1];
	strcpy(hostC, host.c_str());

	/* get IP by host */
	char hostname[NI_MAXHOST];
	char servInfo[NI_MAXSERV];
	std::string errMsg;
	struct addrinfo hints, *res;
	struct in_addr addr;
	int err;

	WSAData data;
	WSAStartup(MAKEWORD(2, 0), &data);

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;

	if ((err = getaddrinfo(host_.c_str(), NULL, &hints, &res)) != 0)
	{
		errMsg = "";
		if (err == 11001)
		{
			errMsg = "Host not found";
		}
		/*throw std::runtime_error(
				"Error Code " + std::to_string(err)
						+ (errMsg.length() > 0 ? ": " + errMsg : ""));*/
	}

	try
	{
		addr.S_un = ((struct sockaddr_in *) (res->ai_addr))->sin_addr.S_un;
	} catch (const std::exception& e)
	{
		throw std::runtime_error(
				"Could not connect to the internet." + std::string(e.what()));
	}
	freeaddrinfo(res);
	WSACleanup();

	//setup a socket and connection tools
	memset((char*) &servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = *((unsigned long*) &(addr.S_un));
	servAddr.sin_port = htons(port);
	listenFlag = false;

	/* get ip address string */
	DWORD dwRetval;
	dwRetval = getnameinfo((struct sockaddr *) &servAddr,
			sizeof(struct sockaddr), hostname,
			NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);

	if (dwRetval == 0)
	{
		std::cout << "ip address: " << hostname << std::endl;
	}
}

void WIN_SOCKET::sockSetup()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		throw std::runtime_error("WSAStartup failed.");
		//return 1;
	}

	//open stream oriented socket with internet address
	//also keep track of the socket descriptor
	serverSd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSd < 0)
	{
		throw std::runtime_error("Error establishing the server socket");
	}
}

void WIN_SOCKET::sockConnect()
{
	//try to connect...
	int status = connect(serverSd, (sockaddr*) &servAddr, sizeof(servAddr));
	if (status != 0)
	{
		throw std::runtime_error("Error connecting to socket!");
	}
}

void WIN_SOCKET::sockBind()
{
	//bind the socket to its local address
	int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr,
			sizeof(servAddr));
	if (bindStatus < 0)
	{
		throw std::runtime_error("Error binding socket to local address");
	}
}

void WIN_SOCKET::sockListen(std::function<void(WIN_SOCKET*)>* listenCB)
{
	//listen for up to 5 requests at a time
	listen(serverSd, 5);
	//receive a request from client using accept
	//we need a new address to connect with the client
	sockaddr_in newSockAddr;
	socklen_t newSockAddrSize = sizeof(newSockAddr);
	//accept, create a new socket descriptor to
	//handle the new connection with client
	acceptSd = accept(serverSd, (sockaddr *) &newSockAddr, &newSockAddrSize);
	if (acceptSd < 0)
	{
		throw std::runtime_error("Error accepting request from client!");
	}
	listenFlag = true;
	//lets keep track of the session time
	//std::chrono::system_clock::time_point t1, t2;
	unsigned int elapsedTime = 0;
	//t1 = std::chrono::system_clock::now();
	//also keep track of the amount of data sent as well
	int bytesRead, bytesWritten = 0;
	char msg[1500];
	while (listenFlag)
	{
		//receive a message from the client (listen)
		std::cout << "Awaiting client response..." << std::endl;
		memset(&msg, 0, sizeof(msg));	//clear the buffer
		bytesRead += recv(acceptSd, (char*) &msg, sizeof(msg), 0);
		if (!strcmp(msg, "exit"))
		{
			std::cout << "Client has quit the session" << std::endl;
			break;
		}
		std::cout << "Client: " << msg << std::endl;
		std::cout << ">";
		std::string data;
		std::getline(std::cin, data);
		memset(&msg, 0, sizeof(msg)); //clear the buffer
		strcpy(msg, data.c_str());
		if (data == "exit")
		{
			//send to the client that server has closed the connection
			send(acceptSd, (char*) &msg, strlen(msg), 0);
			break;
		}
		//send the message to client
		bytesWritten += send(acceptSd, (char*) &msg, strlen(msg), 0);
		(*listenCB)(this);
	}
	//we need to close the socket descriptors after we're all done
	/*t2 = std::chrono::system_clock::now();
	 elapsedTime = (int) std::chrono::duration_cast < std::chrono::seconds
	 > (t2 - t1).count();*/
	closesocket(acceptSd);
	closesocket(serverSd);
	std::cout << "********Session********" << std::endl;
	std::cout << "Bytes written: " << bytesWritten << " Bytes read: "
			<< bytesRead << std::endl;
	std::cout << "Elapsed time: " << elapsedTime << " secs" << std::endl;
	std::cout << "Connection closed..." << std::endl;
}

void WIN_SOCKET::sockLoop(std::function<void(WIN_SOCKET*)>* listenCB)
{
	int bytesRead, bytesWritten = 0;
	//std::chrono::system_clock::time_point t1, t2;
	unsigned int elapsedTime = 0;
	//t1 = std::chrono::system_clock::now();
	char msg[1500];
	while (1)
	{
		std::cout << ">";
		std::string data;
		std::getline(std::cin, data);
		memset(&msg, 0, sizeof(msg)); //clear the buffer
		strcpy(msg, data.c_str());
		if (data == "exit")
		{
			send(serverSd, (char*) &msg, strlen(msg), 0);
			break;
		}
		bytesWritten += send(serverSd, (char*) &msg, strlen(msg), 0);
		std::cout << "Awaiting server response..." << std::endl;
		memset(&msg, 0, sizeof(msg)); //clear the buffer
		bytesRead += recv(serverSd, (char*) &msg, sizeof(msg), 0);
		if (!strcmp(msg, "exit"))
		{
			std::cout << "Server has quit the session" << std::endl;
			break;
		}
		std::cout << "Server: " << msg << std::endl;
		(*listenCB)(this);
	}
	/*t2 = std::chrono::system_clock::now();
	 elapsedTime = (int) std::chrono::duration_cast < std::chrono::seconds
	 > (t2 - t1).count();*/
	closesocket(serverSd);
	std::cout << "********Session********" << std::endl;
	std::cout << "Bytes written: " << bytesWritten << " Bytes read: "
			<< bytesRead << std::endl;
	std::cout << "Elapsed time: " << elapsedTime << " secs" << std::endl;
	std::cout << "Connection closed" << std::endl;
}

#endif
