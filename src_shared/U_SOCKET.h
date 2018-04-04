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
#ifndef U_SOCKET_H
#define U_SOCKET_H

#include <cstring>       // strlen
#include <iostream>
#include <locale>
#include <string>
#include <stdio.h>
#include <fstream>
#include <stdexcept>     // std::runtime_error
#include <functional>
#include <string.h>

#if defined __linux__ || defined __CYGWIN__
#include "POSIX_SOCKET.h"
#define UU_SOCKET POSIX_SOCKET
#elif defined _WIN32
#include "WIN_SOCKET.h"
#define UU_SOCKET WIN_SOCKET
#elif defined __APPLE__
#include "APPLE_SOCKET.h"
#define UU_SOCKET APPLE_SOCKET
#endif

#define BUFFER_SIZE 1024

class U_SOCKET_client {
private:
	UU_SOCKET *sockObjPtr;
	static void CBA(std::function<void(U_SOCKET_client*)> CB,
	UU_SOCKET* sock_);

public:
	U_SOCKET_client();
	U_SOCKET_client(UU_SOCKET *sock_);
	int id();
	void on(std::string event, std::function<void(U_SOCKET_client*)> CB);
	/*
	 * socket.write(data[, encoding][, callback])
	 */
	void connect(int port_, std::string host_);
	void write(std::string msg_, std::function<void(U_SOCKET_client*)>* CB);
	void commandStr(std::string str);
	std::string msgLast();
	std::string errorMsgLast();
};

class U_SOCKET_addr {
public:
	int port;
	std::string family;
	std::string address;
};

class U_SOCKET_close {
public:
	bool success;
};

class U_SOCKET_broadcast {

};

/**
 @class U_SOCKET
 Uses libraries for a basic winsock application.
 A collection of HTTP methods.
 */
class U_SOCKET {
private:
	UU_SOCKET *sockObjPtr;
	std::vector<U_SOCKET_client> *srvConnections;
	static void CBA(std::function<void(U_SOCKET*)> CB,
	UU_SOCKET* sock_);

public:
	/*
	 * equivalent to Node.js:
	 * server = new net.Server([options][, connectionListener]);
	 * server = net.createServer([options][, connectionListener]);
	 * client = new net.Socket([options]);
	 *
	 */
	U_SOCKET();
	U_SOCKET(UU_SOCKET *sock_);
	/*
	 * Returns an object with port, family, and address properties:
	 * { port: 12346, family: 'IPv4', address: '127.0.0.1' }
	 * obj = server.address();
	 */
	U_SOCKET_addr* address();
	/*
	 * server.close([callback]);
	 */
	U_SOCKET_close* close();
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
	U_SOCKET_broadcast broadcast(std::string msg_);
	void on(std::string event, std::function<void(U_SOCKET*)> CB);
	void commandStr(std::string str);
	U_SOCKET_client* clientLast();
	std::string errorMsgLast();
};

#endif
