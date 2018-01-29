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

#include <iostream>
#include <string>
#include <functional>

/* threads */
#if defined _WIN32
#undef _GLIBCXX_HAS_GTHREADS
#include "../src_shared/include/mingw.thread.h"
#include <mutex>
#include "../src_shared/include/mingw.mutex.h"
#include "../src_shared/include/mingw.condition_variable.h"
#include <atomic>
#include <assert.h>
#else
#include <thread>
#endif
/* threads end */

#include "../src_shared/stdin_thread.h"
#include "../src_shared/U_SOCKET.h"

void socketLoop(U_SOCKET* socketServer);
void conCB(U_SOCKET* socketServer, int sockNum);
void dconCB(U_SOCKET* socketServer, int sockNum);
void errCB(U_SOCKET* socketServer, int sockNum);
void rcvCB(U_SOCKET* socketServer, int sockNum);
void loopCB(U_SOCKET* socketServer, int sockNum);

int main()
{
	U_SOCKET* socketServer;
	try
	{
		socketServer = new U_SOCKET(9573);
		socketServer->sockSetup();
	} catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}

	std::function<void(std::string)> CB = [&socketServer](std::string str)
	{	socketServer->stdinListen(str);};
	std::thread first(socketLoop, socketServer);
	std::thread second(stdinListen, &CB);
	while (1)
	{
		/* do timed events and scheduled send() here */
	}
	return 0;
}

void socketLoop(U_SOCKET* socketServer)
{
	try
	{
		std::cout << "Binding..." << std::endl;
		socketServer->sockBind();
		std::cout << "Listening..." << std::endl;
		std::function<void(U_SOCKET*, int)>* conCBF = new std::function<
				void(U_SOCKET*, int)>(&conCB);
		std::function<void(U_SOCKET*, int)>* dconCBF = new std::function<
				void(U_SOCKET*, int)>(&dconCB);
		std::function<void(U_SOCKET*, int)>* errCBF = new std::function<
				void(U_SOCKET*, int)>(&errCB);
		std::function<void(U_SOCKET*, int)>* rcvCBF = new std::function<
				void(U_SOCKET*, int)>(&rcvCB);
		std::function<void(U_SOCKET*, int)>* loopCBF = new std::function<
				void(U_SOCKET*, int)>(&loopCB);
		socketServer->on("connect", conCBF);
		socketServer->on("end", dconCBF);
		socketServer->on("error", errCBF);
		socketServer->on("data", rcvCBF);
		socketServer->on("loop", loopCBF);
		socketServer->sockListen();
	} catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}

void conCB(U_SOCKET* socketServer, int sockNum)
{
	std::cout << "Client Connected! #" << sockNum << std::endl;
}

void dconCB(U_SOCKET* socketServer, int sockNum)
{
	std::cout << "Client Disonnected! #" << sockNum << std::endl;
}

void errCB(U_SOCKET* socketServer, int sockNum)
{
	std::cout << "Client Error! #" << sockNum << std::endl;
}

void rcvCB(U_SOCKET* socketServer, int sockNum)
{
	std::cout << "Client Data Received! #" << sockNum << std::endl;
}

void loopCB(U_SOCKET* socketServer, int sockNum)
{
	std::cout << "Loop Complete! #" << sockNum << std::endl;
}

