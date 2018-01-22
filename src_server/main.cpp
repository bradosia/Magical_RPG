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
#include "../src_shared/U_SOCKET.h"

void listenCB(U_SOCKET* socketServer);

int main()
{
	U_SOCKET* socketServer;
	try
	{
		socketServer = new U_SOCKET(9573);
		socketServer->sockSetup();
		std::cout << "Binding..." << std::endl;
		socketServer->sockBind();
		std::cout << "Listening..." << std::endl;
		socketServer->sockListen(
				new std::function<void(U_SOCKET*)>(&listenCB));
	} catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}

void listenCB(U_SOCKET* socketServer)
{

}
