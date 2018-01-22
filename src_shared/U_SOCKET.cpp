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

U_SOCKET::U_SOCKET(int port_)
{
	sockObjPtr = new UU_SOCKET(port_);
}

U_SOCKET::U_SOCKET(std::string host_, int port_)
{
	sockObjPtr = new UU_SOCKET(host_, port_);
}

U_SOCKET::U_SOCKET(UU_SOCKET *sock_)
{
	sockObjPtr = sock_;
}

void U_SOCKET::sockSetup()
{
	sockObjPtr->sockSetup();
}

void U_SOCKET::sockConnect()
{
	sockObjPtr->sockConnect();
}

void U_SOCKET::sockBind()
{
	sockObjPtr->sockBind();
}

void U_SOCKET::sockListen(std::function<void(U_SOCKET*)>* listenCB)
{
	std::function<void(UU_SOCKET*)> listenCB2 = [&listenCB](UU_SOCKET *sock_){(*listenCB)(new U_SOCKET(sock_));};
	sockObjPtr->sockListen(&listenCB2);
}

void U_SOCKET::sockLoop(std::function<void(U_SOCKET*)>* listenCB)
{
	std::function<void(UU_SOCKET*)> listenCB2 = [&listenCB](UU_SOCKET *sock_){(*listenCB)(new U_SOCKET(sock_));};
	sockObjPtr->sockLoop(&listenCB2);
}

void U_SOCKET::sendFromServer(std::string data){
	sockObjPtr->sendFromServer(data);
}
