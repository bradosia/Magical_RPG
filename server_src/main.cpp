//============================================================================
// Name        : Ravioli Racing
// Author      : Branden Lee
// Version     : 0.01
// Copyright   : GPL
// Description : C++ Racing gameLogicPtr
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
#include <mingw.thread.h>
#include <mutex>
#include <mingw.mutex.h>
#include <mingw.condition_variable.h>
#include <atomic>
#include <assert.h>
#else
#include <thread>
#endif
/* threads end */

#include <asyncConsole/asyncConsole.h>
#include <xSock/xSock.h>
#include <json.hpp>
#include <MR_logic/cLogic.h>

void serverListen(xSock* socketServer);
/* callbacks */
void closeCB(xSock* socketServer);
void connectionCB(xSock* socketServer);
void errorCB(xSock* socketServer);
void listeningCB(xSock* socketServer);

void clientCloseCB(xSockClient* socketClient);
void clientConnectCB(xSockClient* socketClient);
void clientDataCB(xSockClient* socketClient);
void clientErrorCB(xSockClient* socketClient);

// Magical RPG Logic
cLogic* gameLogicPtr;

int main() {
	xSock* socketServer = new xSock();

	std::thread first(serverListen, socketServer);
	std::function<void(std::string)> CB = [&socketServer](std::string str)
	{	socketServer->commandStr(str);};
	std::thread second(asyncConsole::listen, CB);
	asyncConsole::signalInit();
	while (1) {
		/* do timed events and scheduled send() here */
	}
	return 0;
}

void serverListen(xSock* socketServer) {
	try {
		socketServer->on("close", std::function<void(xSock*)>(&closeCB));
		socketServer->on("connection",
				std::function<void(xSock*)>(&connectionCB));
		socketServer->on("error", std::function<void(xSock*)>(&errorCB));
		socketServer->on("listening",
				std::function<void(xSock*)>(&listeningCB));
		socketServer->listen(9573, "localhost"); // 16777343
		//socketServer->listen(9573, "127.0.0.1"); // 16777343
		//socketServer->listen(80, "www.newgrounds.com");
	} catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
}

void closeCB(xSock* socketServer) {
	std::cout << "Server Closed" << std::endl;
}

void connectionCB(xSock* socketServer) {
	xSockClient* socketClient = socketServer->clientLast();
	std::cout << "Player Connected: " << socketClient->id() << std::endl;
	//gameLogicPtr.newPlayer();

	socketClient->on("close",
			std::function<void(xSockClient*)>(&clientCloseCB));
	socketClient->on("connect",
			std::function<void(xSockClient*)>(&clientConnectCB));
	socketClient->on("data",
			std::function<void(xSockClient*)>(&clientDataCB));
	socketClient->on("error",
			std::function<void(xSockClient*)>(&clientErrorCB));
}

void errorCB(xSock* socketServer) {
	std::cout << socketServer->errorMsgLast() << std::endl;
}

void listeningCB(xSock* socketServer) {
	xSock_addr* sockAddr = socketServer->address();
	std::cout << "Listening on port: " << sockAddr->port << " address: "
			<< sockAddr->address << std::endl;
}

void clientCloseCB(xSockClient* socketClient) {
	std::cout << "Player disconnected: " << socketClient->id() << std::endl;
//gameLogicPtr->removePlayer(socketClient->id());
}
void clientConnectCB(xSockClient* socketClient) {
	std::cout << "Player connected!: " << socketClient->id() << std::endl;
}
void clientDataCB(xSockClient* socketClient) {
	std::cout << "Player data: " << socketClient->id() << std::endl;
	std::string msg = socketClient->msgLast();
	std::cout << "Client: " << socketClient->id() << " Received Message: "
			<< msg << std::endl;
	//gameLogicPtr->updatePlayer(socketClient->id(),x,y,angle);
}
void clientErrorCB(xSockClient* socketClient) {
	std::cout << "Player error: " << socketClient->id() << std::endl;
}
