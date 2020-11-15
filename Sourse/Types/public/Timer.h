#pragma once

#include "MathMacros.h"
#include <thread>
#include <time.h>
//#include <ctime>
//#include <chrono>


#define THREAD_SLEEP(time_ms) std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));

//#define T_MSC(sec) ((long long int)(sec * 1000))
//#define T_SEC(msc) ((TIME_SEC)(msc / 1000.f))

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