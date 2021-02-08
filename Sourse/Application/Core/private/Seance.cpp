#pragma once

#include "Core/Seance.h"
#include "Core/Operator.h"
#include "UI/Window.h"
#include "Object.h"

Seance::Seance(Str* Path) {

  if (/*file specified*/ false) {  // load project

    OnRead(/*file path*/);

  } else {  // Load defaults

    initOps(this);

    Path->trim(Range(0, Path->rfind('\\', Range(0, Path->length))));
    *Path += Str("Configuration\\");

    Window* win2 = NEW_DBG(Window) Window(Path, &prefferences.operators);

    project.windows.add(win2);
  }
}

Seance::~Seance() {
  project.collection.del();
  prefferences.operators.del();
  project.windows.del();
  op_threads.del();
}

void Seance::OnWrite(/*file path*/) {}

void Seance::OnRead(/*file path*/) {}

Window* Project::C_actWin() {

  FOREACH_NODE(Window, (&windows), win_node) {
    if (win_node->Data->IsActive()) {
      return win_node->Data;
    }
  }
  return nullptr;
}

Project::~Project() {}

Prefferences::~Prefferences() {}
