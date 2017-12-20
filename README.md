# Libraries Used

Glew - The OpenGL Extension Wrangler Library<BR>
The OpenGL Extension Wrangler Library (GLEW) is a cross-platform open-source C/C++ extension loading library. 
GLEW provides efficient run-time mechanisms for determining which OpenGL extensions are supported on the target platform. 
OpenGL core and extension functionality is exposed in a single header file. 
GLEW has been tested on a variety of operating systems, including Windows, Linux, Mac OS X, FreeBSD, Irix, and Solaris.<BR>
http://glew.sourceforge.net/<BR><BR>

GLUT - OpenGL Utility Toolkit<BR>
GLUT (pronounced like the glut in gluttony) is the OpenGL Utility Toolkit, a window system independent toolkit for writing OpenGL programs. 
It implements a simple windowing application programming interface (API) for OpenGL. 
GLUT makes it considerably easier to learn about and explore OpenGL programming. 
GLUT provides a portable API so you can write a single OpenGL program that works across all PC and workstation OS platforms.<BR>
https://www.opengl.org/resources/libraries/glut/<BR><BR>

Corona<BR>
Corona is an image input/output library that can read, write, and manipulate image files in just a few lines of code. 
It can write PNG and TGA files, and read PNG, JPEG, PCX, BMP, TGA, and GIF. 
Corona was designed to be easy to use, and exports a straightforward C++ API. 
With just a few lines of C++, you can add image loading to your application.<BR>
http://corona.sourceforge.net/<BR><BR>

BASS audio library<BR>
BASS is an audio library for use in software on several platforms. 
Its purpose is to provide developers with powerful and efficient sample, stream (MP3, MP2, MP1, OGG, WAV, AIFF, custom generated, and more via OS codecs and add-ons), MOD music (XM, IT, S3M, MOD, MTM, UMX), MO3 music (MP3/OGG compressed MODs), and recording functions. 
All in a compact DLL that won't bloat your distribution. <BR>
https://www.un4seen.com/<BR><BR>

# Dependencies

## Debian
- g++

## Windows
- cygwin

# Build Instructions

## Debian
```sh
cd Ravioli_Racing\Release
g++ ../src/Ravioli_Racing.cpp ../src/POSIX_HTTP.cpp -o Ravioli_Racing
```

## Windows
```sh
cd Ravioli_Racing\Release
make
```

# Run Server

## Debian
```sh
cd Ravioli_Racing\Release
./Ravioli_Racing
```

## Windows
```sh
cd Ravioli_Racing\Release
Ravioli_Racing
```
