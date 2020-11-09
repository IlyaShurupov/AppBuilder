#include "public/Logic.h"

#include "public/Operator.h"

int MainLoop(Context* C) {
  while (true) {
    Window* actw = C->getActiveWindow();
    actw->UpdEventState();
    if (actw->IsEvent()) {
      continue;  // TODO: go sleep
    }

    FOREACH_NODE(Operator, (&C->Operators), op_node) {
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
    actw->Draw(C);
  }

  return 0;
}