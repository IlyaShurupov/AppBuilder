#pragma once

#include "public/Seance.h"
#include "RayCast.h"
#include "Types.h"
#include "public/Operator.h"
#include "public/Window.h"
//#include "public/Print.h"

Seance::Seance(Str* Path) {

  if (/*file specified*/ false) {

    // load project
    OnRead(/*file path*/);
  } else {

    // Create dummy
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

  //return windows[0];

  FOREACH_NODE(Window, (&windows), win_node) {
    if (win_node->Data->IsActive()) {
      return win_node->Data;
    }
  }
  //post_MSG(CMSGType::ERRORtype, "Active Window Not Found");
  return nullptr;
}
