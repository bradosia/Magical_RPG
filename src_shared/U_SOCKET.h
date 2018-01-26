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
#ifndef U_SOCKET_H
#define U_SOCKET_H

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
#include "POSIX_SOCKET.h"
#define UU_SOCKET POSIX_SOCKET
#elif defined _WIN32
#include "WIN_SOCKET.h"
#define UU_SOCKET WIN_SOCKET
#elif defined __APPLE__
#include "APPLE_SOCKET.h"
#define UU_HTTP APPLE_SOCKET
#endif

#define BUFFER_SIZE 1024

/**
 @class U_SOCKET
 Uses libraries for a basic winsock application.
 A collection of HTTP methods.
 */
class U_SOCKET
{
private:
	UU_SOCKET *sockObjPtr;
public:
	U_SOCKET(int port);
	U_SOCKET(std::string host_, int port);
	U_SOCKET(UU_SOCKET *sock_);
	void sockSetup();
	void sockConnect();
	void sockBind();
	void sockListen(std::function<void(U_SOCKET*)>* listenCB);
	void sockLoop(std::function<void(U_SOCKET*)>* listenCB);
	void stdinListen(std::string str);
	void sendFromServer(std::string data);
};

#endif
