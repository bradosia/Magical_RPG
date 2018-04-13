/*
 * This class will create a cross-platform simple window manipulation
 * features include:
 * - get the window openGL created
 * - change window title
 * - change window icon
 * */
#ifndef XWIN_H
#define XWIN_H
#include <string>

#if defined __linux__ || defined __CYGWIN__
#include <X11/Xlib.h>
#elif defined _WIN32
#include <windows.h>
#elif defined __APPLE__
/*
 * Apple GUI compilation
 * g++ -framework Cocoa -o test1 test1.cpp
 * */
#include <CoreFoundation/CoreFoundation.h>
#include <objc/objc.h>
#include <objc/objc-runtime.h>
#endif

class XHAND {
private:
#if defined _WIN32
	HWND handle;
#endif
public:
#if defined _WIN32
	XHAND(HWND winHandle);
#endif
};

class XWIN {
public:
	static XHAND* find(std::string className, std::string winName);
	static void removeConsole();
};
#endif
