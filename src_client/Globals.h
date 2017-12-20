#define WIN32_LEAN_AND_MEAN
// Having issues using the shared library DLL for glew
#define GLEW_STATIC
#define GLUT_NO_LIB_PRAGMA
#define GLUT_NO_WARNING_DISABLE
#define GLUT_BUILDING_LIB

#include <iostream>
#include <typeinfo>
#include <windef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include "include/GL/glew.h" //las comillas sirven para que ese fichero se carge buscandolo en la propia carpeta del proyecto
#include "include/GL/glut.h"
