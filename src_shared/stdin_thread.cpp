#include "stdin_thread.h"

void stdinListen(std::function<void(std::string)>* CB)
{
	std::string temp;
	std::cout << "test" << std::endl;
	while (1)
	{
		std::cin >> temp;
		(*CB)(temp);
	}
}
