//============================================================================
// Name        : xSock
// Author      : Brad Lee
// Version     : 0.01
// Copyright   : GPL
// Description : Cross-platform Sockets for C++
//
// IDE: Eclipse Version: Neon.2 Release (4.6.2)
// Requires MinGW for windows, GCC for linux, and XCode for mac
//============================================================================
#ifndef X_HTTP_H
#define X_HTTP_H

#include <cstring>       // strlen
#include <iostream>
#include <locale>
#include <string>
#include <fstream>
#include <stdexcept>     // std::runtime_error
#include <functional>
#include <string.h>

#if defined __linux__ || defined __CYGWIN__
#include "../xSock/src/POSIX_HTTP.h"
#define P_HTTP POSIX_HTTP
#elif defined _WIN32
#include "../xSock/src/WIN_HTTP.h"
#define P_HTTP WIN_HTTP
#elif defined __APPLE__
#include "../xSock/src/APPLE_HTTP.h"
#define P_HTTP APPLE_HTTP
#endif

/**
 @class xHTTP
 Uses libraries for a basic winsock application.
 A collection of HTTP methods.
 */
class xHTTP
{
private:
	P_HTTP *httpObjPtr;
public:
	xHTTP();
	int socket_connect(char *host, int port);
	std::string getWebsite(std::string url, std::string path);
	std::string html(std::string response);
};

#endif
