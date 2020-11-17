#pragma once

#include <time.h>
#include <chrono>
#include <thread>
#include "MathMacros.h"

#define GETTIMEMSC()                                                                                         \
  (TIME_MS)(                                                                                                 \
      std::chrono::duration_cast<std::chrono::milliseconds>(                                                 \
          std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()) \
              .time_since_epoch())                                                                           \
          .count())

#define THREAD_SLEEP(time_ms) std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));

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