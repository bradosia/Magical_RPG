//============================================================================
// Name        : xSock
// Author      : Brad Lee
// Version     : 0.01
// Copyright   : GPL
// Description : Cross-platform Sockets for C++
//
// IDE: Eclipse Version: Neon.2 Release (4.6.2)
// Requires MinGW for windows, GCC for linux, and XCode for mac
//============================================================================
#ifndef X_SOCK_H
#define X_SOCK_H

#include <cstring>       // strlen
#include <iostream>
#include <locale>
#include <string>
#include <fstream>
#include <stdexcept>     // std::runtime_error
#include <functional>
#include <string.h>

#if defined __linux__ || defined __CYGWIN__
#include "../xSock/src/POSIX_SOCKET.h"
#define P_SOCK POSIX_SOCKET
#elif defined _WIN32
#include "../xSock/src/WIN_SOCKET.h"
#define P_SOCK WIN_SOCKET
#elif defined __APPLE__
#include "../xSock/src/APPLE_SOCKET.h"
#define P_SOCK APPLE_SOCKET
#endif

#define BUFFER_SIZE 1024

class xSockClient {
private:
	P_SOCK *sockObjPtr;
	static void CBA(std::function<void(xSockClient*)> CB,
	P_SOCK* sock_);

public:
	xSockClient();
	xSockClient(P_SOCK *sock_);
	int id();
	void on(std::string event, std::function<void(xSockClient*)> CB);
	/*
	 * socket.write(data[, encoding][, callback])
	 */
	void connect(int port_, std::string host_);
	void write(std::string msg_, std::function<void(xSockClient*)> CB);
	void commandStr(std::string str);
	std::string msgLast();
	std::string errorMsgLast();
};

class xSock_addr {
public:
	int port;
	std::string family;
	std::string address;
};

class xSock_close {
public:
	bool success;
};

class xSock_broadcast {

};

/**
 @class xSock
 Uses libraries for a basic winsock application.
 A collection of HTTP methods.
 */
class xSock {
private:
	P_SOCK *sockObjPtr;
	std::vector<xSockClient> *srvConnections;
	static void CBA(std::function<void(xSock*)> CB,
	P_SOCK* sock_);

public:
	/*
	 * equivalent to Node.js:
	 * server = new net.Server([options][, connectionListener]);
	 * server = net.createServer([options][, connectionListener]);
	 * client = new net.Socket([options]);
	 *
	 */
	xSock();
	xSock(P_SOCK *sock_);
	/*
	 * Returns an object with port, family, and address properties:
	 * { port: 12346, family: 'IPv4', address: '127.0.0.1' }
	 * obj = server.address();
	 */
	xSock_addr* address();
	/*
	 * server.close([callback]);
	 */
	xSock_close* close();
	/*
	 * server.getConnections(callback);
	 * callback = function(err, count){};
	 */
	void getConnections(std::function<void(std::string err, int count)>* CB);
	/*
	 * server.listen(PORT, HOST);
	 */
	void listen(int port_, std::string host_);
	void listen(int port_);
	void broadcast(std::string msg_,
			std::function<void(xSock_broadcast*)> CB);
	void write(std::string msg_, int clientId_,
			std::function<void(xSock*)> CB);
	void on(std::string event, std::function<void(xSock*)> CB);
	void commandStr(std::string str);
	xSockClient* clientLast();
	std::string errorMsgLast();
};

#endif
