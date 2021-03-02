#pragma once

typedef long long int TIME_MS;

struct Timer {

  TIME_MS start;
  TIME_MS duration;

  Timer();
  Timer(TIME_MS time);
  bool timeout();
  void reset();
  TIME_MS past();
  TIME_MS remain();

  float ease_in(TIME_MS duration = 0);
  float ease_out(TIME_MS duration = 0);
};

void TreadSleep(TIME_MS duration);