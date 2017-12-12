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

#include <iostream>
#include "POSIX_HTTP.h"

int main()
{
	std::string response;
	std::cout << "Connecting..." << std::endl; // prints !!!Hello World!!!
	response = POSIX_HTTP::getWebsite("www.dummies.com", "/");
	response = POSIX_HTTP::html(response);
	std::cout << response;
	std::cin.get();
	return 0;
}
