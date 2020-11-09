#include "public/Logic.h"

#include "public/Context.h"
#include "public/Win32API.h"

int AppMainLoop(Context* C) {

  while (true) {

    C->event.Update(&C->event);
    if (!C->event.IsEvent()) {
      continue;  // TODO: go sleep
    }

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
    C->window.SysOutput();
  }

  return 0;
}