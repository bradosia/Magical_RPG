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

void U_SOCKET::sockListen()
{
	sockObjPtr->sockListen();
}

void U_SOCKET::sockLoop()
{
	sockObjPtr->sockLoop();
}

void U_SOCKET::on(std::string event, std::function<void(U_SOCKET*, int)>* CB)
{
	std::function<void(UU_SOCKET*, int)> CBF =
			[&CB](UU_SOCKET *sock_, int sockNum)
			{	(*CB)(new U_SOCKET(sock_), sockNum);};
	sockObjPtr->on(event, &CBF);
}

void U_SOCKET::stdinListen(std::string str)
{
	sockObjPtr->stdinListen(str);
}

void U_SOCKET::sendFromServer(std::string data)
{
	sockObjPtr->sendFromServer(data);
}
