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

U_HTTP::U_HTTP()
{
	httpObjPtr = new UU_HTTP();
}

int U_HTTP::socket_connect(char *host, int port_)
{
	return httpObjPtr->socket_connect(host, port_);
}

std::string U_HTTP::getWebsite(std::string url, std::string path)
{
	return httpObjPtr->getWebsite(url, path);
}

std::string U_HTTP::html(std::string response)
{
	return httpObjPtr->html(response);
}
