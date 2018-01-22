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
#ifndef WIN_SOCKET_H
#define WIN_SOCKET_H
#ifdef _WIN32
#define __USE_MINGW_ANSI_STDIO 0 // clears error: multiple definition of `vsnprintf'

#include <cstring>       // strlen
#include <iostream>
#include <locale>
#include <string>
#include <stdio.h>
#include <fstream>
#include <stdexcept>     // std::runtime_error
#include <functional>
#include <chrono>

/* windows sock */
#define _WIN32_WINNT 0x6000 // getaddrinfo and freeaddrinfo
#include <winsock2.h>
#include <ws2tcpip.h> // getaddrinfo


#define BUFFER_SIZE 1024

/**
 @class WinHTTP
 Uses libraries for a basic winsock application.
 A collection of HTTP methods.
 */
class WIN_SOCKET
{
private:
	sockaddr_in servAddr;
	int serverSd;
	int acceptSd;
	bool listenFlag;
	std::string host;
	u_short port;

public:
	WIN_SOCKET(int port);
	WIN_SOCKET(std::string host_, int port);
	void sockSetup();
	void sockConnect();
	void sockBind();
	void sockListen(std::function<void(WIN_SOCKET*)>* listenCB);
	void sockLoop(std::function<void(WIN_SOCKET*)>* listenCB);
	void sendFromServer(std::string data);
};

#endif
#endif
