#pragma once
#include "public/Seance.h"

Seance::Seance(std::string* basePath) {

  if (/*file specified*/ false) {

    // load project
    OnRead(/*file path*/);
  } else {
    // Create dummy
    initOps(this);

    basePath->erase(basePath->rfind('\\') + 1);
    std::string configfolder = *basePath + "\\Configuration\\";


    // window
    //Window* win1 = new Window(&filepath, &prefferences.operators);
    //win1->ToggleConsole();

    Window* win2 = new Window(&configfolder, &prefferences.operators);
    //win2->ToggleConsole();
    //project.windows.add(win1);
    project.windows.add(win2);
    //win.

    //initOps();
  }
}

Seance::~Seance() {}

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
