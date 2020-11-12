
#include "public/Seance.h"

int main() {
  // TOD: Redesign operator logic, Handle args

  // enable memory debuging
#ifdef _DEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  // Create Seance
  Seance& C = *new Seance(/*project file if user opens .proj file*/);

  // Main loop: Handle events -> run operators -> show result
  while (true) {
    // Handle events for each window
    FOREACH_NODE(Window, (&C.project.windows), win_node) {
      Window* win = win_node->Data;
      win->ProcessEvents(&C.exec_queue);
    }

    // TODO: prioritize commands by sorting them in the list

    // Run Operators from queu (This is where the fun happends)
    FOREACH_NODE(ExecComand, (&C.exec_queue), op_node) {
      ExecComand* command = op_node->Data;

      switch (command->op_event) {
        case OpEventState::MODAL_EVENT:
          if (command->op->Modal)
            command->op->Modal(&C, command->op, command->modal_event);
          break;

        case OpEventState::INVOKE:
          if (command->op->Invoke)
            command->op->Invoke(&C, command->op);
          break;

        case OpEventState::EXECUTE:
          assert(command->op->Execute);
          command->op->Execute(&C, command->op);
          break;
      }
    }

    // Draw each window & show updates
    FOREACH_NODE(Window, (&C.project.windows), win_node) {
      Window* win = win_node->Data;
      win->Draw();
      win->SendBuffToSystem();
    }
  }

  // Print out mem leaks
#ifdef _DEBUG
  _CrtDumpMemoryLeaks();
#endif
}