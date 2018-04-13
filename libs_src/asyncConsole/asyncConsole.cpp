#include "asyncConsole.h"

void asyncConsole::listen(std::function<void(std::string)> CB) {
	std::string temp;
	while (1) {
		std::getline(std::cin, temp);
		CB(temp);
	}
}

void asyncConsole::signalCB(int sig_number) {
	if (sig_number == SIGINT) {
		std::cout << "SIGINT was caught!\n";
		//((o).(*fn))(EXIT_SUCCESS);
	}
	/* else if (sig_number == SIGPIPE) {
	 printf("SIGPIPE was catched!\n");
	 shutdown_properly(EXIT_SUCCESS);
	 }*/
}
int asyncConsole::signalInit() {
	signal(SIGABRT, asyncConsole::signalCB);
	/*
	 if (signal(SIGINT, &sa, 0) != 0) {
	 perror("sigaction()");
	 return -1;
	 }
	 if (signal(SIGPIPE, &sa, 0) != 0) {
	 perror("sigaction()");
	 return -1;
	 }*/
	return 0;
}
