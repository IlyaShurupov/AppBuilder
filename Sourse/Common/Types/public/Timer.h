#pragma once

#include <ctime>
#include <windows.h>

class _Time
{
public:
	_Time();
	_Time(double timer);
	~_Time();

	bool timeout();
	void reset_start();
	void set_timer(double time);
	long time_pased();
	void sleep(double ms);

private:
	long start;
	long timer;
};
