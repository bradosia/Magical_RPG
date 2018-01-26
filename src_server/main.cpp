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
void conCB(U_SOCKET* socketServer);
void dconCB(U_SOCKET* socketServer);
void rcvCB(U_SOCKET* socketServer);

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
		std::function<void(U_SOCKET*)>* conCBF =new std::function<void(U_SOCKET*)>(&conCB);
		std::function<void(U_SOCKET*)>* dconCBF =new std::function<void(U_SOCKET*)>(&dconCB);
		std::function<void(U_SOCKET*)>* rcvCBF =new std::function<void(U_SOCKET*)>(&rcvCB);
		socketServer->sockListen(conCBF, dconCBF, rcvCBF);
	} catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}

void conCB(U_SOCKET* socketServer)
{
	std::cout << "Client Connected!" << std::endl;
}

void dconCB(U_SOCKET* socketServer)
{
	std::cout << "Client Disonnected!" << std::endl;
}

void rcvCB(U_SOCKET* socketServer)
{
	std::cout << "Client Data Received!" << std::endl;
}
