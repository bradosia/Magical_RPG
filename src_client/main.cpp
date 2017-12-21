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
#undef _GLIBCXX_HAS_GTHREADS
#include "../src_shared/include/mingw.thread.h"
#include <mutex>
#include "../src_shared/include/mingw.mutex.h"
#include "../src_shared/include/mingw.condition_variable.h"
#include <atomic>
#include <assert.h>

#include "../src_shared/U_SOCKET.h"
#include "globals.h"
#include "cGame.h"

void listenCB(U_SOCKET* socketServer);

void serverConnect();

cGame Game;

void AppReshape(int w, int h)
{
	Game.Reshape(w, h);
}
void AppRender()
{
	Game.Render();
}
void AppKeyboard(unsigned char key, int x, int y)
{
	Game.ReadKeyboard(key, x, y, true);
}
void AppKeyboardUp(unsigned char key, int x, int y)
{
	Game.ReadKeyboard(key, x, y, false);
}
void AppSpecialKeys(int key, int x, int y)
{
	Game.ReadSpecialKeyboard(key, x, y, true);
}
void AppSpecialKeysUp(int key, int x, int y)
{
	Game.ReadSpecialKeyboard(key, x, y, false);
}
void AppMouse(int button, int state, int x, int y)
{
	Game.ReadMouse(button, state, x, y);
}
void AppIdle()
{
	if (!Game.Loop())
		exit(0);
}

int main(int argc, char** argv)
{
	std::thread first(serverConnect);

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
	glutCreateWindow("Ravioli Racing");

	/*glutGameModeString("800x600:32");
	 glutEnterGameMode();*/

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
	if (GLEW_OK != GlewInitResult)
	{
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}

	//Game initializations
	Game.Init(1);

	//Application loop
	glutMainLoop();

	return 0;
}

void listenCB(U_SOCKET* socketServer)
{

}

void serverConnect()
{
	U_SOCKET* socketServer;
	try
	{
		std::cout << "New Socket..." << std::endl;
		socketServer = new U_SOCKET("localhost", 9573);
		std::cout << "Setup..." << std::endl;
		socketServer->sockSetup();
		std::cout << "Connecting..." << std::endl;
		socketServer->sockConnect();
		std::cout << "Looping..." << std::endl;
		socketServer->sockLoop(new std::function<void(U_SOCKET*)>(&listenCB));
	} catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}

