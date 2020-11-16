#pragma once

#include "MathMacros.h"
#include <thread>
#include <time.h>

#define GETTIMEMSC() (TIME_MS)((long double)time(0)*1000)
#define THREAD_SLEEP(time_ms) std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));

typedef long long int TIME_MS;

struct Timer {
	
	TIME_MS start;
	TIME_MS duration;

	Timer::Timer(TIME_MS time);
	bool Timer::timeout();
	void Timer::reset();
	TIME_MS Timer::past();
	TIME_MS Timer::remain();
};