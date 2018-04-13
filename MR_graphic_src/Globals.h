
#define WIN32_LEAN_AND_MEAN
// Having issues using the shared library DLL for glew
#define GLEW_STATIC
#define MINGW_HAS_SECURE_API

#include <iostream>
#include <typeinfo>
#if defined _WIN32
#include <windef.h>
#include <stdarg.h>
#include <stdio.h>
#endif
#include <math.h>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include "../client_src/xwin.h"
/* GLEW
 * The OpenGL Extension Wrangler Library
 */
#include <GL/glew.h>
/* GLFW 3.2.1
 * Open Source, multi-platform library for OpenGL
 */
#include <GLFW/glfw3.h>
/* FBX Software Development Kit
 * Adaptable File Formats for 3D Animation Software | Autodesk
 */
#include <fbxsdk.h>
/* ReactPhysics3D
 * Open-source physics engine
 */
#include <reactphysics3d/reactphysics3d.h>
/* Dear ImGui
 * Bloat-free Immediate Mode Graphical User interface for C++ with minimal dependencies
 */
#include "../imgui_src/imgui.h"
/* Corona
 * an image input/output library that can read, write, and manipulate image files
 */
#include <corona.h>
/* LodePNG
 * PNG encoder and decoder in C and C++.
 */
#include <lodepng/lodepng.h>
/* BASS
 * an audio library for use in software on several platforms
 */
#include <bass.h>
