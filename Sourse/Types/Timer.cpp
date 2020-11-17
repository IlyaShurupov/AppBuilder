
#include "public/Timer.h"

Timer::Timer() {
  duration = 0;
  start = 0;
}

Timer::Timer(TIME_MS duration) {
  start = GETTIMEMSC();
  this->duration = duration;
}

bool Timer::timeout() {
  printf("\n ----------- %lli", GETTIMEMSC());
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

float Timer::ease_in(TIME_MS duration) {

  if (!duration) {
    duration = this->duration;
  }

  float x = (1.f / duration) * past();

  float out = (1.1f * x) / (x + 0.1f);
  CLAMP(out, 0, 1);
  //printf("\n %lfp", ((long double)time(0) * 1000));
  //printf("\n %f", out);
  return out;
}

float Timer::ease_out(TIME_MS duration) {
  if (!duration) {
    duration = this->duration;
  }

  float x = (1.f / duration) * past();

  float out = (0.1f *(1 - x)) / (x + 0.1f);
  CLAMP(out, 0, 1);
  return out;
}
