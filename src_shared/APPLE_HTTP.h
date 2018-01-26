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
#ifndef APPLE_HTTP_H
#define APPLE_HTTP_H
#if defined __APPLE__

#include <cstring>       // strlen
#include <iostream>
#include <locale>
#include <string>
#include <stdio.h>
#include <fstream>
#include <stdexcept>     // std::runtime_error
#include <functional>
#include <chrono>
#include <vector>
#include <signal.h>
//#include <io.h>

// apple specific
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>      // close()
#include <netdb.h>       // getaddrinfo() and freeaddrinfo()
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
// apple end

#define BUFFER_SIZE 1024

// void bzero(void *s, size_t n);
#define bzero(s, n) memset((s), 0, (n))

// void bcopy(const void *s1, void *s2, size_t n);
#define bcopy(s1, s2, n) memmove((s2), (s1), (n))
/**
@class WinHTTP
Uses libraries for a basic winsock application.
A collection of HTTP methods.
*/
class APPLE_HTTP
{
public:
	APPLE_HTTP();
	int socket_connect(char *host, int port);
	std::string getWebsite(std::string url, std::string path);
	std::string html (std::string response);
};
#endif
#endif
