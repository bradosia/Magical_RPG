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
#include "U_SOCKET.h"

void U_SOCKET_client::CBA(std::function<void(U_SOCKET_client*)> CB,
UU_SOCKET* sock_) {
	(CB)(new U_SOCKET_client(sock_));
}

U_SOCKET_client::U_SOCKET_client() {
	sockObjPtr = new UU_SOCKET();
}

U_SOCKET_client::U_SOCKET_client(UU_SOCKET *sock_) {
	sockObjPtr = sock_;
}

int U_SOCKET_client::id() {
	return sockObjPtr->getSockId();
}

void U_SOCKET_client::on(std::string event,
		std::function<void(U_SOCKET_client*)> CB) {
	sockObjPtr->on(event,
			std::bind(&U_SOCKET_client::CBA, CB, std::placeholders::_1));
}

void U_SOCKET_client::connect(int port_, std::string host_) {
	sockObjPtr->connecting(port_, host_);
}

void U_SOCKET_client::write(std::string msg_,
		std::function<void(U_SOCKET_client*)>* CB) {

}

void U_SOCKET_client::commandStr(std::string str) {
	sockObjPtr->commandClientStr(str);
}

std::string U_SOCKET_client::msgLast() {
	return sockObjPtr->rcvMsgLast();
}

std::string U_SOCKET_client::errorMsgLast() {
	return sockObjPtr->errorMsgLast();
}

void U_SOCKET::CBA(std::function<void(U_SOCKET*)> CB,
UU_SOCKET* sock_) {
	(CB)(new U_SOCKET(sock_));
}

U_SOCKET::U_SOCKET() {
	sockObjPtr = new UU_SOCKET();
}

U_SOCKET::U_SOCKET(UU_SOCKET *sock_) {
	sockObjPtr = sock_;
}

U_SOCKET_addr* U_SOCKET::address() {
	U_SOCKET_addr* addr = new U_SOCKET_addr;
	addr->port = sockObjPtr->getPort();
	addr->family = sockObjPtr->getFamily();
	addr->address = sockObjPtr->getAddress();
	return addr;
}

U_SOCKET_close* U_SOCKET::close() {
	U_SOCKET_close* status = new U_SOCKET_close;
	status->success = true;
	return status;
}

void U_SOCKET::getConnections(
		std::function<void(std::string err, int count)>* CB) {

}

void U_SOCKET::listen(int port_, std::string host_) {
	sockObjPtr->listening(port_, host_);
}
void U_SOCKET::listen(int port_) {
	sockObjPtr->listening(port_);
}

void U_SOCKET::on(std::string event, std::function<void(U_SOCKET*)> CB) {
	sockObjPtr->on(event, std::bind(&U_SOCKET::CBA, CB, std::placeholders::_1));
}

void U_SOCKET::commandStr(std::string str) {
	sockObjPtr->commandSrvStr(str);
}

U_SOCKET_client* U_SOCKET::clientLast() {
	return new U_SOCKET_client(sockObjPtr->clientLast());
}

std::string U_SOCKET::errorMsgLast() {
	return "";
}
