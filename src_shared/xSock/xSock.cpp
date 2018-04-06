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
#include "xSock.h"

void xSockClient::CBA(std::function<void(xSockClient*)> CB,
P_SOCK* sock_) {
	(CB)(new xSockClient(sock_));
}

xSockClient::xSockClient() {
	sockObjPtr = new P_SOCK();
}

xSockClient::xSockClient(P_SOCK *sock_) {
	sockObjPtr = sock_;
}

int xSockClient::id() {
	return sockObjPtr->getSockId();
}

void xSockClient::on(std::string event,
		std::function<void(xSockClient*)> CB) {
	sockObjPtr->on(event,
			std::bind(&xSockClient::CBA, CB, std::placeholders::_1));
}

void xSockClient::connect(int port_, std::string host_) {
	sockObjPtr->connecting(port_, host_);
}

void xSockClient::write(std::string msg_,
		std::function<void(xSockClient*)> CB) {
	sockObjPtr->sockSend(msg_);
	CB(this);
}

void xSockClient::commandStr(std::string str) {
	sockObjPtr->commandClientStr(str);
}

std::string xSockClient::msgLast() {
	return sockObjPtr->rcvMsgLast();
}

std::string xSockClient::errorMsgLast() {
	return sockObjPtr->errorMsgLast();
}

void xSock::CBA(std::function<void(xSock*)> CB,
P_SOCK* sock_) {
	(CB)(new xSock(sock_));
}

xSock::xSock() {
	sockObjPtr = new P_SOCK();
}

xSock::xSock(P_SOCK *sock_) {
	sockObjPtr = sock_;
}

xSock_addr* xSock::address() {
	xSock_addr* addr = new xSock_addr;
	addr->port = sockObjPtr->getPort();
	addr->family = sockObjPtr->getFamily();
	addr->address = sockObjPtr->getAddress();
	return addr;
}

xSock_close* xSock::close() {
	xSock_close* status = new xSock_close;
	status->success = true;
	return status;
}

void xSock::getConnections(
		std::function<void(std::string err, int count)>* CB) {

}

void xSock::listen(int port_, std::string host_) {
	sockObjPtr->listening(port_, host_);
}
void xSock::listen(int port_) {
	sockObjPtr->listening(port_);
}

void xSock::broadcast(std::string msg_,
		std::function<void(xSock_broadcast*)> CB) {
	sockObjPtr->sockSendAll(msg_);
	xSock_broadcast* broadcastObj = new xSock_broadcast;
	CB(broadcastObj);
}
void xSock::write(std::string msg_, int clientId_,
		std::function<void(xSock*)> CB) {
	sockObjPtr->sockSend(msg_, clientId_);
	CB(this);
}

void xSock::on(std::string event, std::function<void(xSock*)> CB) {
	sockObjPtr->on(event, std::bind(&xSock::CBA, CB, std::placeholders::_1));
}

void xSock::commandStr(std::string str) {
	sockObjPtr->commandSrvStr(str);
}

xSockClient* xSock::clientLast() {
	return new xSockClient(sockObjPtr->clientLast());
}

std::string xSock::errorMsgLast() {
	return "";
}
