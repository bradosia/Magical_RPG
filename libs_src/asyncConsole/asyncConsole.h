#ifndef STDIN_THREAD_H
#define STDIN_THREAD_H

#include <iostream>
#include <string>
#include <functional>
#include <signal.h>
#include <io.h>

class asyncConsole {
public:
	static void listen(std::function<void(std::string)> CB);
	static void signalCB(int sig_number);
	static int signalInit();
};

#endif
