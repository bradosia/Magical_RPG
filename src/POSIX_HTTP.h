/*
Branden Lee, Anh Truong, Alexander Morfin, and Michael Wu
CIS 22C
Fall 2017
Final Project

Used Microsoft Visual Studio 2017
Windows SDK Version: 10.0.16299.0
Use SDK Version: 10.0.15063.0 for De Anza Computers
USE DOXYGEN COMPLIANT DOCUMENTATION
*/
#ifndef POSIX_HTTP_H
#define POSIX_HTTP_H

#include <cstring>
#include <string>
#include <iostream>
#include <locale>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */

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
class POSIX_HTTP
{
public:
	static int socket_connect(char *host, in_port_t port);
	static std::string getWebsite(std::string url, std::string path);
	static std::string html (std::string response);
};

#endif
