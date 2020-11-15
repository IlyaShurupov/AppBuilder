
#include "public/Timer.h"

Timer::Timer(TIME_MS duration) {
  start = clock();
  this->duration = duration;
}

bool Timer::timeout() {
  return duration < clock() - start;
}

void Timer::reset() {
  start = clock();
}

TIME_MS Timer::past() {
  return clock() - start;
}

TIME_MS Timer::remain() {
  return duration - (clock() - start);
}