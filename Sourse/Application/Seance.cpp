#pragma once
#include "public/Seance.h"
#include "public/Print.h"

Seance::Seance(std::string* basePath) {

  if (/*file specified*/ false) {

    // load project
    OnRead(/*file path*/);
  } else {

    // Create dummy
    initOps(this);

    basePath->erase(basePath->rfind('\\') + 1);
    std::string configfolder = *basePath + "\\Configuration\\";

    Window* win2 = DBG_NEW Window(&configfolder, &prefferences.operators);

    project.windows.add(win2);

  }
}

Seance::~Seance() {
  op_threads.del();
  project.collection.del();
  project.windows.del();
  prefferences.operators.del();
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
  post_MSG(CMSGType::ERRORtype, "Active Window Not Found");
  return nullptr;
}
