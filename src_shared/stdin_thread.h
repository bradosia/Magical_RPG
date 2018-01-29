#ifndef STDIN_THREAD_H
#define STDIN_THREAD_H

#include <iostream>
#include <string>
#include <functional>

void stdinListen(std::function<void(std::string)>* CB);

#endif
