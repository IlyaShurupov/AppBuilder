#pragma once
#include "public/Seance.h"

Seance::Seance(/*project file*/) {

  if (/*file specified*/ false) {

    // load project
    OnRead(/*file path*/);
  } else {
    // Create dummy
    initOps(this);

    // Create dummy user keymap
    KeyCondition* kc1 = new KeyCondition();
    kc1->input_idname = "SPACE";
    kc1->trigger_state = InputState::PRESSED;

    KeyCondition* kc2 = new KeyCondition();
    kc2->input_idname = "SPACE";
    kc2->trigger_state = InputState::RELEASED;


    Shortcut* shcut2 = new Shortcut();
    shcut2->conditions.add(kc2);
    shcut2->modal_event.idname = "FINISHED";

    OpBindings* opb = new OpBindings();
    opb->invoke.conditions.add(kc1);
    opb->modal_keymap.add(shcut2);
    opb->op_name = "dummy_op";

    prefferences.key_map.map.add(opb);


    // window
    Window* win = new Window(&prefferences.key_map, &prefferences.operators);
    project.windows.add(win);
    //win.

    //initOps();
  }
}

Seance::~Seance() {}

void Seance::OnWrite(/*file path*/) {}

void Seance::OnRead(/*file path*/) {}