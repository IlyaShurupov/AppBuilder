
#include "public/Context.h"

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
      win->ProcessEvents();
    }

    // TODO: prioritize operators by sorting them in the list

    // Run Operators from queu (This is where the fun happends)
    FOREACH_NODE(Operator, (&C.exec_queue), op_node) {
      Operator* op = op_node->Data;

      // Checks if it's possible to execute the command
      //op->Poll(&C);

      /*
      switch (op->State) {
        case OpState::FINISHED:
          Op->Poll(&C, Op);
          break;

        case OpState::RUNNING:
          Op->Modal(&C, Op);
          break;

        case OpState::EXECUTE:
          Op->Execute(&C, Op);
          break;

        case OpState::INVOKE:
          Op->Invoke(&C, Op);
          break;
      }
      */
    }

    // Draw each window & show updates
    FOREACH_NODE(Window, (&C.project.windows), win_node) {
      Window* win = win_node->Data;
      win->Draw();
      win->SendBuffToSystem();
    }
  }

  // Print out diagnostic
#ifdef _DEBUG
  _CrtDumpMemoryLeaks();
#endif
}