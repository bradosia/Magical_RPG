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

#define PRODUCTION 0
#define DEBUG 1

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
#endif

#include "../src_shared/stdin_thread.h"
#include "../src_shared/U_SOCKET.h"
#include "globals.h"
#include "cGame.h"

void closeCB(U_SOCKET* socketServer);
void connectionCB(U_SOCKET* socketServer);
void errorCB(U_SOCKET* socketServer);
void listeningCB(U_SOCKET* socketServer);
void serverConnect(U_SOCKET* socketServer);

cGame Game;

void AppReshape(int w, int h) {
	Game.Reshape(w, h);
}
void AppRender() {
	Game.Render();
}
void AppKeyboard(unsigned char key, int x, int y) {
	Game.ReadKeyboard(key, x, y, true);
}
void AppKeyboardUp(unsigned char key, int x, int y) {
	Game.ReadKeyboard(key, x, y, false);
}
void AppSpecialKeys(int key, int x, int y) {
	Game.ReadSpecialKeyboard(key, x, y, true);
}
void AppSpecialKeysUp(int key, int x, int y) {
	Game.ReadSpecialKeyboard(key, x, y, false);
}
void AppMouse(int button, int state, int x, int y) {
	Game.ReadMouse(button, state, x, y);
}
void AppIdle() {
	if (!Game.Loop())
		exit(0);
}

void init(int argc, char** argv) {
	U_SOCKET* socketServer = new U_SOCKET();

	//XWIN::removeConsole(); // closing the openGL window wont close the main process
	std::string projectName = "Magical RPG";
	std::thread first(serverConnect, socketServer);
	std::function<void(std::string)> CB = [&socketServer](std::string str)
	{	socketServer->stdinListen(str);};
	std::thread second(stdinListen, &CB);

	int res_x, res_y, pos_x, pos_y;

	//GLUT initialization
	glutInit(&argc, argv);

	//RGBA with double buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

	//Create centered window
	res_x = glutGet(GLUT_SCREEN_WIDTH);
	res_y = glutGet(GLUT_SCREEN_HEIGHT);
	pos_x = (res_x >> 1) - (GAME_WIDTH >> 1);
	pos_y = (res_y >> 1) - (GAME_HEIGHT >> 1);

	glutInitWindowPosition(pos_x, pos_y);
	glutInitWindowSize(GAME_WIDTH, GAME_HEIGHT);
	//glutInitWindowSize(512, 288);

	glutCreateWindow(projectName.c_str());
	//XWIN::find(NULL, projectName); //get the window handler

	glutGameModeString("800x600:32");
	//glutEnterGameMode();

	//Make the default cursor disappear
	//glutSetCursor(GLUT_CURSOR_NONE);
	//Register callback functions
	glutReshapeFunc(AppReshape);
	glutDisplayFunc(AppRender);
	glutKeyboardFunc(AppKeyboard);
	glutKeyboardUpFunc(AppKeyboardUp);
	glutSpecialFunc(AppSpecialKeys);
	glutSpecialUpFunc(AppSpecialKeysUp);
	glutMouseFunc(AppMouse);
	glutIdleFunc(AppIdle);

	//GLEW initialization
	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult) {
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}

	//Game initializations
	Game.Init(1);

	//Application loop
	glutMainLoop();
}

#if DEBUG
int main(int argc, char** argv) {
	init(argc, argv);
	return 0;
}
#endif

#if PRODUCTION && defined _WIN
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		PSTR lpCmdLine, INT nCmdShow)
{
	init(0, 0);
	return 0;
}
#endif

void closeCB(U_SOCKET* socketServer) {
	std::cout << "Server Closed" << std::endl;
}

void connectionCB(U_SOCKET* socketServer) {
	U_SOCKET_client* socketClient = socketServer->clientLast();
	//Game.newPlayer();
	std::function<void(U_SOCKET_client*)> clientCloseCB =
			[&Game](U_SOCKET_client* socketClient)
			{	std::cout << "Player disconnected: " << socketClient->id() << std::endl;
				//Game->removePlayer(socketClient->id());
			};
	std::function<void(U_SOCKET_client*)> clientConnectCB =
			[&Game](U_SOCKET_client* socketClient)
			{	std::cout << "Player connected: " << socketClient->id() << std::endl;
			};
	std::function<void(U_SOCKET_client*)> clientDataCB =
			[&Game](U_SOCKET_client* socketClient)
			{
				std::string msg = socketClient->msgLast();
				std::cout << "Client: " << socketClient->id() << " Received Message: " << msg << std::endl;
				//Game->updatePlayer(socketClient->id(),x,y,angle);
			};
	std::function<void(U_SOCKET_client*)> clientErrorCB =
			[&Game](U_SOCKET_client* socketClient)
			{
				std::cout << "Player error: " << socketClient->id() << std::endl;
			};

	socketClient->on("close", &clientCloseCB);
	socketClient->on("connect", &clientConnectCB);
	socketClient->on("data", &clientDataCB);
	socketClient->on("error", &clientErrorCB);
}

void errorCB(U_SOCKET* socketServer) {
	std::cout << socketServer->errorMsgLast() << std::endl;
}

void listeningCB(U_SOCKET* socketServer) {
	U_SOCKET_addr* sockAddr = socketServer->address();
	std::cout << "Listening on port: " << sockAddr->port << "address: "
			<< sockAddr->address << std::endl;
}

void serverConnect(U_SOCKET* socketServer) {
	try {
		socketServer->on("close", new std::function<void(U_SOCKET*)>(&closeCB));
		socketServer->on("connection",
				new std::function<void(U_SOCKET*)>(&connectionCB));
		socketServer->on("error", new std::function<void(U_SOCKET*)>(&errorCB));
		socketServer->on("listening",
				new std::function<void(U_SOCKET*)>(&listeningCB));
		socketServer->listen(9573, "localhost");

		/*
		 std::cout << "New Socket..." << std::endl;
		 socketServer = new U_SOCKET("localhost", 9573);
		 std::cout << "Setup..." << std::endl;
		 socketServer->sockSetup();
		 std::cout << "Connecting..." << std::endl;
		 socketServer->sockConnect();
		 std::cout << "Looping..." << std::endl;
		 socketServer->sockLoop(new std::function<void(U_SOCKET*)>(&listenCB));
		 */
	} catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
}
