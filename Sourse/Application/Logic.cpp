#include "public/Logic.h"

#include <cstdio>

#include "public/Context.h"
#include "public/Win32API.h"

int AppMainLoop(Context* C) {
  while (true) {
    C->SysInput(C->SysH, &C->event);

    // FOREACH_STACK(ExecCommand, Operator, actw->OpExecQueue) {
    //}

    FOREACH_NODE(Operator, (&C->operators), op_node) {
      Operator* Op = op_node->Data;

      switch (Op->State) {
        case OpState::FINISHED:
          Op->Poll(C, Op);
          break;

        case OpState::RUNNING:
          Op->Modal(C, Op);
          break;

        case OpState::EXECUTE:
          Op->Execute(C, Op);
          break;

        case OpState::INVOKE:
          Op->Invoke(C, Op);
          break;
      }
    }

    // Draw all data into frame buffer
    C->window.Draw(C);

    // Just show frame buff
    C->SysOutput(C->SysH);
  }

  return 0;
}