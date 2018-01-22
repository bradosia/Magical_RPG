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
#ifndef POSIX_SOCKET_H
#define POSIX_SOCKET_H
#if defined __linux__ || defined __CYGWIN__

#include <cstring>       // strlen
#include <iostream>
#include <locale>
#include <string>
#include <stdio.h>
#include <fstream>
#include <stdexcept>     // std::runtime_error
#include <functional>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>      // close()
#include <netdb.h>       // getaddrinfo() and freeaddrinfo()
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

/**
 @class WinHTTP
 Uses libraries for a basic winsock application.
 A collection of HTTP methods.
 */
class POSIX_SOCKET
{
private:
#ifdef __linux__
	sockaddr_in servAddr;
	int serverSd;
	int acceptSd;
	bool listenFlag;
	std::string host;
	in_port_t port;
#endif
public:
	POSIX_SOCKET(int port);
	POSIX_SOCKET(std::string host_, int port);
	void sockSetup();
	void sockConnect();
	void sockBind();
	void sockListen(std::function<void(POSIX_SOCKET*)>* listenCB);
	void sockLoop(std::function<void(POSIX_SOCKET*)>* listenCB);
};

#endif
#endif
