
#include "public/Timer.h"



Timer::Timer(TIME_MS duration) {
  start = GETTIMEMSC();
  this->duration = duration;
}

bool Timer::timeout() {
  return duration < GETTIMEMSC() - start;
}

void Timer::reset() {
  start = GETTIMEMSC();
}

TIME_MS Timer::past() {
  return GETTIMEMSC() - start;
}

TIME_MS Timer::remain() {
  return duration - (GETTIMEMSC() - start);
}