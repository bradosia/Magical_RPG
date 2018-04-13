#include "../client_src/xwin.h"

#if defined _WIN32
XHAND::XHAND(HWND winHandle) {
	handle = winHandle;
}
#endif

XHAND* XWIN::find(std::string className, std::string winName) {
#if defined _WIN32
	return new XHAND(FindWindow(NULL, winName.c_str()));
#endif
}

void XWIN::removeConsole() {
#if defined _WIN32
	FreeConsole();
#endif
}
