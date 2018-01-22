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
#ifndef U_HTTP_H
#define U_HTTP_H

#include <cstring>       // strlen
#include <iostream>
#include <locale>
#include <string>
#include <stdio.h>
#include <fstream>
#include <stdexcept>     // std::runtime_error
#include <functional>
#include <string.h>

#if defined __linux__ || defined __CYGWIN__
#include "POSIX_HTTP.h"
#define UU_HTTP POSIX_HTTP
#elif defined _WIN32
#include "WIN_HTTP.h"
#define UU_HTTP WIN_HTTP
#endif

/**
 @class U_HTTP
 Uses libraries for a basic winsock application.
 A collection of HTTP methods.
 */
class U_HTTP
{
private:
	UU_HTTP *httpObjPtr;
public:
	U_HTTP();
	int socket_connect(char *host, int port);
	std::string getWebsite(std::string url, std::string path);
	std::string html(std::string response);
};

#endif
