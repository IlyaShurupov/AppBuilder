#include "timer.h"


_Time::_Time()
{
	start = clock();
	timer = -1;
}

_Time::_Time(double timer_ms)
{
	start = clock();
	timer = timer_ms;
}

_Time::~_Time()
{
}

bool _Time::timeout()
{
	float time = clock();
	if (start + timer <= clock()) {
		return true;
	}
	return false;
}

void _Time::reset_start()
{
	start = clock();
}

void _Time::set_timer(double time)
{
	timer = time;
}

long _Time::time_pased()
{

	return clock() - start;
}

void _Time::sleep(double ms)
{
	Sleep(DWORD(ms));
}
