#pragma once
#include "public/Seance.h"

Seance::Seance(/*project file*/) {

  if (/*file specified*/ false) {

    // load project
    OnRead(/*file path*/);
  } else {
    // Create dummy

    //initOps(this);
    //initOps();
  }
}

Seance::~Seance() {}

void Seance::OnWrite(/*file path*/) {}

void Seance::OnRead(/*file path*/) {}