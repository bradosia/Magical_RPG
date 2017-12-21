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

#ifdef __linux__
#include "POSIX_HTTP.h"
#define UU_HTTP POSIX_HTTP
#elif _WIN32
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
public:
	static int socket_connect(char *host, int port);
	static std::string getWebsite(std::string url, std::string path);
	static std::string html(std::string response);
};

#endif
