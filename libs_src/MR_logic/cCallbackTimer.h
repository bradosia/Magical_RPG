#ifndef C_CALLBACK_TIMER_H
#define C_CALLBACK_TIMER_H

#include <ctime>
#include <chrono>
#include <functional>

#include "../MR_logic/cActor.h"


class cCallbackTimer {
public:
	std::function<void(cActor*)> CB;
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point endTime;
};

#endif
