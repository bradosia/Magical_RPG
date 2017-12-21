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
#include "U_HTTP.h"

int U_HTTP::socket_connect(char *host, int port_)
{
	return UU_HTTP::socket_connect(host, port_);
}

std::string U_HTTP::getWebsite(std::string url, std::string path)
{
	return UU_HTTP::getWebsite(url, path);
}

std::string U_HTTP::html(std::string response)
{
	return UU_HTTP::html(response);
}
