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
#include "xHTTP.h"

xHTTP::xHTTP()
{
	httpObjPtr = new P_HTTP();
}

int xHTTP::socket_connect(char *host, int port_)
{
	return httpObjPtr->socket_connect(host, port_);
}

std::string xHTTP::getWebsite(std::string url, std::string path)
{
	return httpObjPtr->getWebsite(url, path);
}

std::string xHTTP::html(std::string response)
{
	return httpObjPtr->html(response);
}
