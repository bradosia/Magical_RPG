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

U_SOCKET_client::U_SOCKET_client() {
	sockObjPtr = new UU_SOCKET();
}

int U_SOCKET_client::id() {
	return 0;
}

void U_SOCKET_client::on(std::string event,
		std::function<void(U_SOCKET_client*)>* CB) {
	std::function<void(UU_SOCKET*)> CBF = [&CB](UU_SOCKET *sock_)
	{	(*CB)(new U_SOCKET_client());};
	sockObjPtr->onClient(event, &CBF);
}

void U_SOCKET_client::write(std::string msg_,
		std::function<void(U_SOCKET_client*)>* CB) {

}

std::string U_SOCKET_client::msgLast() {

}

std::string msgLast() {
	return "";
}

U_SOCKET::U_SOCKET() {
	sockObjPtr = new UU_SOCKET();
}

U_SOCKET::U_SOCKET(UU_SOCKET *sock_) {
	sockObjPtr = sock_;
}

void U_SOCKET::sockSetup() {
	sockObjPtr->sockSetup();
}

void U_SOCKET::sockConnect() {
	sockObjPtr->sockConnect();
}

void U_SOCKET::sockBind() {
	sockObjPtr->sockBind();
}

void U_SOCKET::sockListen() {
	sockObjPtr->sockListen();
}

void U_SOCKET::sockLoop() {
	sockObjPtr->sockLoop();
}

U_SOCKET_addr* U_SOCKET::address() {

}

U_SOCKET_close* U_SOCKET::close() {

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

void U_SOCKET::on(std::string event, std::function<void(U_SOCKET*)>* CB) {
	std::function<void(UU_SOCKET*)> CBF = [&CB](UU_SOCKET *sock_)
	{	(*CB)(new U_SOCKET(sock_));};
	sockObjPtr->on(event, &CBF);
}

void U_SOCKET::stdinListen(std::string str) {
	sockObjPtr->stdinListen(str);
}

U_SOCKET_client* U_SOCKET::clientLast() {
}
std::string U_SOCKET::errorMsgLast() {
}
