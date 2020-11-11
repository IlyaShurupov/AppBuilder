#pragma once
#include "public/Context.h"

Seance::Seance(/*project file*/) {

  if (/*file specified*/ false) {

    // load project
    OnRead(/*file path*/);
  } else {
    // Create dummy
  }
}

Seance::~Seance() {}

void Seance::OnWrite(/*file path*/) {}

void Seance::OnRead(/*file path*/) {}