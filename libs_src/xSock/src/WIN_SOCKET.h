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
#ifndef WIN_SOCKET_H
#define WIN_SOCKET_H

#ifdef _WIN32
#define __USE_MINGW_ANSI_STDIO 0 // clears error: multiple definition of `vsnprintf'

#include <cstring>       // strlen
#include <iostream>
#include <locale>
#include <string> // std::string, std::stoi
#include <unordered_map>
#include <fstream>
#include <stdexcept>     // std::runtime_error
#include <functional>
#include <chrono>
#include <vector>
#include "../../xSock/src/StringHelper.h"

/* windows sock */
#define _WIN32_WINNT 0x6000 // getaddrinfo and freeaddrinfo
#define WIN32_LEAN_AND_MEAN
#include <windows.h> // MAKEWORD
#include <winsock2.h>
#include <ws2tcpip.h> // getaddrinfo

#define BUFFER_SIZE 1024
#define NO_SOCKET -1
#define EXIT_FAILURE 1

/* Maximum bytes that can be send() or recv() via net by one call.
 * It's a good idea to test sending one byte by one.
 */
#define MAX_SEND_SIZE 100

/* Size of send queue (messages). */
#define MAX_MESSAGES_BUFFER_SIZE 10

#define SENDER_MAXSIZE 128
#define DATA_MAXSIZE 512
#define MSG_DONTWAIT 0

#define EAGAIN          11      /* Try again */
#define EWOULDBLOCK     EAGAIN  /* Operation would block */

/**
 @class WinHTTP
 Uses libraries for a basic winsock application.
 A collection of HTTP methods.
 */
class WIN_SOCKET {
private:
	int sockId;
	sockaddr_in sockAddr;
	unsigned long sockAddrUn;
	bool listenFlag, connectFlag;
	std::string host;
	std::string hostName;
	std::string sockFamily;
	std::string rcvMsgCurrent;
	std::string sendMsgCurrent;
	std::vector<std::string> rcvMsgList;
	std::vector<std::string> errorMsgList;
	u_short port;
	std::vector<WIN_SOCKET*> connectionList;
	int connectionLastIndex;
	fd_set read_fds;
	fd_set write_fds;
	fd_set except_fds;
	int sendBytes;
	int rcvBytes;
	std::unordered_map<std::string, std::function<void(WIN_SOCKET*)>> CBmap;
	void setHost(std::string host_);
	void setPort(int port_, unsigned long IP_address);
	// socket connections
	void sockSetup();
	void sockClientConnect();
	void sockClientLoop();
	void sockSrvBind();
	void sockSrvListen();
	int cmdRCV();
	int sockSend();
	/** Checks and handles socket connection
	 @pre None
	 @post None
	 @param sockNum socket number connected, unchanged if not connected
	 @return 0 on success, error code on failure
	 */
	int sockCon();
	/** Checks and handles socket disconnection
	 @pre None
	 @post None
	 @param sockNum socket number disconnected, unchanged if not disconnected
	 @return 0 on success, error code on failure
	 */
	int sockDcon();
	int build_fd_sets(fd_set *read_fds, fd_set *write_fds, fd_set *except_fds);

public:
	WIN_SOCKET();
	/* connection & listening */
	void listening(int port_, std::string host_);
	void listening(int port_);
	void connecting(int port_, std::string host_);
	void sockClose();
	/* event listeners */
	void on(std::string event, std::function<void(WIN_SOCKET*)> CB_);
	void emit(std::string event);
	/* get/set */
	int getPort();
	int getSockId();
	std::string getFamily();
	std::string getAddress();
	WIN_SOCKET* clientLast();
	std::string rcvMsgLast();
	std::string errorMsgLast();
	std::string getSendMsgCurrent();
	void setSockId(int sockId_);
	void setSockAddr(sockaddr_in addr_);
	void getHostName(std::string &hostname_, std::string &service_);
	bool activeCon();
	/* string commands */
	void commandSrvStr(std::string str);
	void commandClientStr(std::string str);
	/* network */
	void sockSend(std::string data);
	void sockSend(std::string data, int client_);
	void sockSendAll(std::string data);
};

#endif
#endif
