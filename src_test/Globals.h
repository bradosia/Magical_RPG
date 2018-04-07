#ifndef GLOBALS_H
#define GLOBALS_H

#define WIN32_LEAN_AND_MEAN
// Having issues using the shared library DLL for glew
#define GLEW_STATIC
#define __USE_MINGW_ANSI_STDIO 0 // clears error: multiple definition of `vsnprintf'

#include <iostream>
#include <typeinfo>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include "include/GL/glew.h"
// glfw3
#include "include/GLFW/glfw3.h"
#include "../src_shared/imgui/imgui.h"

#endif
