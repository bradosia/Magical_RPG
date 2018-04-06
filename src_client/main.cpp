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

#include "../src_shared/asyncConsole/asyncConsole.h"
#include "../src_shared/xSock/xSock.h"
#include "../src_shared/json.hpp"
#include "globals.h"
#include "cGame.h"

void clientConnect(xSockClient* socketClient);
/* callbacks */
void closeCB(xSockClient* socketClient);
void connectCB(xSockClient* socketClient);
void dataCB(xSockClient* socketClient);
void endCB(xSockClient* socketClient);
void errorCB(xSockClient* socketClient);
void gameLoopCB(cGame* gameObj, xSockClient* socketClient);
void writeUpdateCB(xSockClient* socketClient);

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
	xSockClient* socketClient = new xSockClient();

	//XWIN::removeConsole(); // closing the openGL window wont close the main process
	std::string projectName = "Magical RPG";
	std::thread first(clientConnect, socketClient);
	std::function<void(std::string)> CB = [&socketClient](std::string str)
	{	socketClient->commandStr(str);};
	std::thread second(asyncConsole::listen, CB);
	asyncConsole::signalInit();

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

void clientConnect(xSockClient* socketClient) {
	try {
		socketClient->on("close",
				std::function<void(xSockClient*)>(&closeCB));
		socketClient->on("connect",
				std::function<void(xSockClient*)>(&connectCB));
		socketClient->on("data",
				std::function<void(xSockClient*)>(&dataCB));
		socketClient->on("end", std::function<void(xSockClient*)>(&endCB));
		socketClient->on("error",
				std::function<void(xSockClient*)>(&errorCB));
		socketClient->connect(9573, "localhost"); // 16777343
	} catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
}

void closeCB(xSockClient* socketClient) {
	std::cout << "Client Closed" << std::endl;
}

void connectCB(xSockClient* socketClient) {
	std::cout << "Player connected: " << socketClient->id() << std::endl;
	Game.getActorMain()->SetTile(8, 5);
	//Game.on("loopEnd", std::bind(&gameLoopCB, std::placeholders::_1, socketClient) );
}

void dataCB(xSockClient* socketClient) {
	std::string msg = socketClient->msgLast();
	std::cout << "Client: " << socketClient->id() << " Received Message: "
			<< msg << std::endl;
	Game.getActorMain()->SetTile(8, 5);
}
void endCB(xSockClient* socketClient) {
	std::cout << "Server issued a disconnection" << std::endl;
}

void errorCB(xSockClient* socketClient) {
	std::cout << "Player error: " << socketClient->id() << std::endl
			<< socketClient->errorMsgLast() << std::endl;
}

void gameLoopCB(cGame* gameObj, xSockClient* socketClient) {
	std::string gameStatus = "P@x:"
			+ std::to_string(gameObj->getActorMain()->GetX()) + ",y:"
			+ std::to_string(gameObj->getActorMain()->GetY());
	socketClient->write(gameStatus,
			std::function<void(xSockClient*)>(&writeUpdateCB));
}

void writeUpdateCB(xSockClient* socketClient) {

}
