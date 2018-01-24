//============================================================================
// Name        : Magical RPG
// Author      : Branden Lee
// Version     : 0.01
// Copyright   : GPL
// Description : C++ OpenGL Sockets Multiplayer RPG
//
// IDE: Eclipse Version: Neon.2 Release (4.6.2)
// Requires Cygwin in windows and GCC in linux
//============================================================================
#ifndef WIN_HTTP_H
#define WIN_HTTP_H
#if defined _WIN32

#include <cstring>       // strlen
#include <iostream>
#include <locale>
#include <string>
#include <stdio.h>
#include <fstream>
#include <stdexcept>     // std::runtime_error
#include <functional>
#include <string.h>

/* windows sock */
#include <winsock2.h>
#include <ws2tcpip.h> // getaddrinfo

#define BUFFER_SIZE 1024

/**
 @class WIN_HTTP
 Uses libraries for a basic winsock application.
 A collection of HTTP methods.
 */
class WIN_HTTP
{
public:
	WIN_HTTP();
	int socket_connect(char *host, int port);
	std::string getWebsite(std::string url, std::string path);
	std::string html(std::string response);
};

#endif
#endif
