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

void clientConnect(U_SOCKET_client* socketClient);
/* callbacks */
void closeCB(U_SOCKET_client* socketClient);
void connectCB(U_SOCKET_client* socketClient);
void dataCB(U_SOCKET_client* socketClient);
void endCB(U_SOCKET_client* socketClient);
void errorCB(U_SOCKET_client* socketClient);

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
	U_SOCKET_client* socketClient = new U_SOCKET_client();

	//XWIN::removeConsole(); // closing the openGL window wont close the main process
	std::string projectName = "Magical RPG";
	std::thread first(clientConnect, socketClient);
	std::function<void(std::string)> CB = [&socketClient](std::string str)
	{	socketClient->commandStr(str);};
	std::thread second(stdinHandle::listen, CB);
	stdinHandle::signalInit();

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

void clientConnect(U_SOCKET_client* socketClient) {
	try {
		socketClient->on("close",
				std::function<void(U_SOCKET_client*)>(&closeCB));
		socketClient->on("connect",
				std::function<void(U_SOCKET_client*)>(&connectCB));
		socketClient->on("data",
				std::function<void(U_SOCKET_client*)>(&dataCB));
		socketClient->on("end", std::function<void(U_SOCKET_client*)>(&endCB));
		socketClient->on("error",
				std::function<void(U_SOCKET_client*)>(&errorCB));
		socketClient->connect(9573, "localhost"); // 16777343
	} catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
}

void closeCB(U_SOCKET_client* socketClient) {
	std::cout << "Client Closed" << std::endl;
}

void connectCB(U_SOCKET_client* socketClient) {
	std::cout << "Player connected: " << socketClient->id() << std::endl;
}

void dataCB(U_SOCKET_client* socketClient) {
	std::string msg = socketClient->msgLast();
	std::cout << "Client: " << socketClient->id() << " Received Message: "
			<< msg << std::endl;
	//Game->updatePlayer(socketClient->id(),x,y,angle);
}
void endCB(U_SOCKET_client* socketClient) {
	std::cout << "Server issued a disconnection" << std::endl;
}

void errorCB(U_SOCKET_client* socketClient) {
	std::cout << "Player error: " << socketClient->id() << std::endl
			<< socketClient->errorMsgLast() << std::endl;
}
