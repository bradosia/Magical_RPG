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
#include "../include/win32_mingw/mingw.thread.h"
#include <mutex>
#include "../include/win32_mingw/mingw.mutex.h"
#include "../include/win32_mingw/mingw.condition_variable.h"
#include <atomic>
#include <assert.h>
#else
#include <thread>
#endif
/* threads end */

#include "../libs_src/asyncConsole/asyncConsole.h"
#include "../libs_src/xSock/xSock.h"
#include "../include/win32_mingw/json.hpp"

#include "Globals.h"
#include <MR_logic/cLogic.h>
#include <MR_graphic/cGraphic.h>

#include "imgui_impl_glfw_gl2.h"

void clientConnect(xSockClient* socketClient);
/* callbacks */
void closeCB(xSockClient* socketClient);
void connectCB(xSockClient* socketClient);
void dataCB(xSockClient* socketClient);
void endCB(xSockClient* socketClient);
void errorCB(xSockClient* socketClient);
void gameLoopCB(cLogic* gameLogicPtr, xSockClient* socketClient);
void writeUpdateCB(xSockClient* socketClient);

cLogic* gameLogicPtr = new cLogic;
cGraphic* gameGraphicPtr = new cGraphic(gameLogicPtr);

void AppKeyboard(GLFWwindow* window, int key, int, int action, int mods) {
	ImGui_ImplGlfw_KeyCallback(window, key, 0, action, mods);
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureKeyboard) {
		gameLogicPtr->ReadKeyboard(key, 0, action, mods);
	}
}
void AppMouse(int button, int state, int x, int y) {
	gameLogicPtr->ReadMouse(button, state, x, y);
}

static void glfw_error_callback(int error, const char* description) {
	std::cout << "Error " << error << ": " << description << std::endl;
}

int init(int argc, char** argv) {
	FbxManager* lSdkManager = NULL;
	xSockClient* socketClient = new xSockClient();

	//XWIN::removeConsole(); // closing the openGL window wont close the main process
	std::string projectName = "Magical RPG";
	std::thread first(clientConnect, socketClient);
	std::function<void(std::string)> CB = [&socketClient](std::string str)
	{	socketClient->commandStr(str);};
	std::thread second(asyncConsole::listen, CB);
	asyncConsole::signalInit();

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;
	GLFWwindow* window = glfwCreateWindow(432 * 3, 432 * 2,
			"ImGui GLFW+OpenGL2 example", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	//GLEW initialization
	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult) {
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}

	//Game initializations
	gameLogicPtr->Init(1);

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void) io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplGlfwGL2_Init(window, true);

	// Setup style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	glfwSetKeyCallback(window, AppKeyboard);

	while (!glfwWindowShouldClose(window)) {
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();
		ImGui_ImplGlfwGL2_NewFrame();

		// 1. Show a simple window.
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
		{
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Text("Hello, world!"); // Display some text (you can use a format string too)
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*) &clear_color); // Edit 3 floats representing a color

			ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our windows open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			if (ImGui::Button("Button")) // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();                            //
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
					1000.0f / ImGui::GetIO().Framerate,
					ImGui::GetIO().Framerate);
		}

		// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
		if (show_another_window) {
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
		if (show_demo_window) {
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z,
				clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		gameLogicPtr->Loop();
		gameGraphicPtr->Reshape(288 * 2, 432 * 2);
		gameGraphicPtr->Render();

		ImGui::Render();
		ImGui_ImplGlfwGL2_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

	}

	// Cleanup
	ImGui_ImplGlfwGL2_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

#if DEBUG
int main(int argc, char** argv) {
	return init(argc, argv);
}
#endif

#if PRODUCTION && defined _WIN
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		PSTR lpCmdLine, INT nCmdShow)
{
	return init(0, 0);
}
#endif

void clientConnect(xSockClient* socketClient) {
	try {
		socketClient->on("close", std::function<void(xSockClient*)>(&closeCB));
		socketClient->on("connect",
				std::function<void(xSockClient*)>(&connectCB));
		socketClient->on("data", std::function<void(xSockClient*)>(&dataCB));
		socketClient->on("end", std::function<void(xSockClient*)>(&endCB));
		socketClient->on("error", std::function<void(xSockClient*)>(&errorCB));
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
	gameLogicPtr->getActorMain()->SetTile(8, 5);
	//gameLogicPtr->on("loopEnd", std::bind(&gameLoopCB, std::placeholders::_1, socketClient) );
}

void dataCB(xSockClient* socketClient) {
	std::string msg = socketClient->msgLast();
	std::cout << "Client: " << socketClient->id() << " Received Message: "
			<< msg << std::endl;
	gameLogicPtr->getActorMain()->SetTile(8, 5);
}
void endCB(xSockClient* socketClient) {
	std::cout << "Server issued a disconnection" << std::endl;
}

void errorCB(xSockClient* socketClient) {
	std::cout << "Player error: " << socketClient->id() << std::endl
			<< socketClient->errorMsgLast() << std::endl;
}

void gameLoopCB(cLogic* gameLogicPtr, xSockClient* socketClient) {
	std::string gameStatus = "P@x:"
			+ std::to_string(gameLogicPtr->getActorMain()->GetX()) + ",y:"
			+ std::to_string(gameLogicPtr->getActorMain()->GetY());
	socketClient->write(gameStatus,
			std::function<void(xSockClient*)>(&writeUpdateCB));
}

void writeUpdateCB(xSockClient* socketClient) {

}
