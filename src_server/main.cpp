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
#include "MR_logic.h"

void serverListen(U_SOCKET* socketServer);
/* callbacks */
void closeCB(U_SOCKET* socketServer);
void connectionCB(U_SOCKET* socketServer);
void errorCB(U_SOCKET* socketServer);
void listeningCB(U_SOCKET* socketServer);

void clientCloseCB(U_SOCKET_client* socketClient);
void clientConnectCB(U_SOCKET_client* socketClient);
void clientDataCB(U_SOCKET_client* socketClient);
void clientErrorCB(U_SOCKET_client* socketClient);

// Magical RPG Logic
MR_logic Game;

int main() {
	U_SOCKET* socketServer = new U_SOCKET();

	std::thread first(serverListen, socketServer);
	std::function<void(std::string)> CB = [&socketServer](std::string str)
	{	socketServer->commandStr(str);};
	std::thread second(stdinHandle::listen, CB);
	stdinHandle::signalInit();
	while (1) {
		/* do timed events and scheduled send() here */
	}
	return 0;
}

void serverListen(U_SOCKET* socketServer) {
	try {
		socketServer->on("close", std::function<void(U_SOCKET*)>(&closeCB));
		socketServer->on("connection",
				std::function<void(U_SOCKET*)>(&connectionCB));
		socketServer->on("error", std::function<void(U_SOCKET*)>(&errorCB));
		socketServer->on("listening",
				std::function<void(U_SOCKET*)>(&listeningCB));
		socketServer->listen(9573, "localhost"); // 16777343
		//socketServer->listen(9573, "127.0.0.1"); // 16777343
		//socketServer->listen(80, "www.newgrounds.com");
	} catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
}

void closeCB(U_SOCKET* socketServer) {
	std::cout << "Server Closed" << std::endl;
}

void connectionCB(U_SOCKET* socketServer) {
	U_SOCKET_client* socketClient = socketServer->clientLast();
	std::cout << "Player Connected: " << socketClient->id() << std::endl;
	//Game.newPlayer();

	socketClient->on("close",
			std::function<void(U_SOCKET_client*)>(&clientCloseCB));
	socketClient->on("connect",
			std::function<void(U_SOCKET_client*)>(&clientConnectCB));
	socketClient->on("data",
			std::function<void(U_SOCKET_client*)>(&clientDataCB));
	socketClient->on("error",
			std::function<void(U_SOCKET_client*)>(&clientErrorCB));
}

void errorCB(U_SOCKET* socketServer) {
	std::cout << socketServer->errorMsgLast() << std::endl;
}

void listeningCB(U_SOCKET* socketServer) {
	U_SOCKET_addr* sockAddr = socketServer->address();
	std::cout << "Listening on port: " << sockAddr->port << " address: "
			<< sockAddr->address << std::endl;
}

void clientCloseCB(U_SOCKET_client* socketClient) {
	std::cout << "Player disconnected: " << socketClient->id() << std::endl;
//Game->removePlayer(socketClient->id());
}
void clientConnectCB(U_SOCKET_client* socketClient) {
	std::cout << "Player connected!: " << socketClient->id() << std::endl;
}
void clientDataCB(U_SOCKET_client* socketClient) {
	std::cout << "Player data: " << socketClient->id() << std::endl;
	std::string msg = socketClient->msgLast();
	std::cout << "Client: " << socketClient->id() << " Received Message: "
			<< msg << std::endl;
	//Game->updatePlayer(socketClient->id(),x,y,angle);
}
void clientErrorCB(U_SOCKET_client* socketClient) {
	std::cout << "Player error: " << socketClient->id() << std::endl;
}
