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
#include "../src/POSIX_SOCKET.h"
#ifdef __linux__

POSIX_SOCKET::POSIX_SOCKET(int port_)
{
	port = (in_port_t)port_;
	//setup a socket and connection tools
	memset((char*) &servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);
	listenFlag = false;
}

POSIX_SOCKET::POSIX_SOCKET(std::string host_, int port_)
{
	port = (in_port_t)port_;
	host = host_;
	char* hostC = new char[host.length() + 1];
	strcpy(hostC, host.c_str());
	struct hostent* host = gethostbyname(hostC);
	//setup a socket and connection tools
	memset((char*) &servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(
			inet_ntoa(*(struct in_addr*) *host->h_addr_list));
	servAddr.sin_port = htons(port);
	listenFlag = false;
}

void POSIX_SOCKET::sockSetup()
{
	//open stream oriented socket with internet address
	//also keep track of the socket descriptor
	serverSd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSd < 0)
	{
		throw std::runtime_error("Error establishing the server socket");
	}
}

void POSIX_SOCKET::sockConnect()
{
	//try to connect...
	int status = connect(serverSd, (sockaddr*) &servAddr, sizeof(servAddr));
	if (status < 0)
	{
		throw std::runtime_error("Error connecting to socket!");
	}
}

void POSIX_SOCKET::sockBind()
{
	//bind the socket to its local address
	int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr,
			sizeof(servAddr));
	if (bindStatus < 0)
	{
		throw std::runtime_error("Error binding socket to local address");
	}
}

void POSIX_SOCKET::sockListen(std::function<void(POSIX_SOCKET*)>* listenCB)
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
	struct timeval start1, end1;
	gettimeofday(&start1, NULL);
	//also keep track of the amount of data sent as well
	int bytesRead, bytesWritten = 0;
	char msg[1500];
	while (listenFlag)
	{
		//receive a message from the client (listen)
		std::cout << "Awaiting client response..." << std::endl;
		memset(&msg, 0, sizeof(msg));//clear the buffer
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
	gettimeofday(&end1, NULL);
	close(acceptSd);
	close(serverSd);
	std::cout << "********Session********" << std::endl;
	std::cout << "Bytes written: " << bytesWritten << " Bytes read: "
	<< bytesRead << std::endl;
	std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs"
	<< std::endl;
	std::cout << "Connection closed..." << std::endl;
}

void POSIX_SOCKET::sockLoop(std::function<void(POSIX_SOCKET*)>* listenCB)
{
	int bytesRead, bytesWritten = 0;
	struct timeval start1, end1;
	gettimeofday(&start1, NULL);
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
	gettimeofday(&end1, NULL);
	close (serverSd);
	std::cout << "********Session********" << std::endl;
	std::cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead
	<< std::endl;
	std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs"
	<< std::endl;
	std::cout << "Connection closed" << std::endl;
}
#endif
