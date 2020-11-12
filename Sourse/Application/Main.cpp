
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
      win->ProcessEvents(&C.op_threads);
    }

    // TODO: prioritize commands by sorting them in the list

    // Run Operators from queu (This is where the fun happends)
    FOREACH_NODE(OpThread, (&C.op_threads), thread_node) {
      OpEventState* op_event = &thread_node->Data->op_event;
      Operator* op = thread_node->Data->op;

      switch (op->state) {

        case OpState::RUNNING_MODAL:
          // keep running
          op->Modal(&C, op, thread_node->Data->modal_event);
          break;

        case OpState::CANCELED:
          C.op_threads.del(thread_node);
          // Todo: undo system
          break;

        case OpState::FINISHED:
          C.op_threads.del(thread_node);
          break;


        case OpState::NONE: {
          if (!op->Poll(&C, op)) {
            C.op_threads.del(thread_node);
            break;
          }
          switch (*op_event) {
            case OpEventState::EXECUTE:   
              op->Execute(&C, op);
              break;

            case OpEventState::INVOKE:
              op->Invoke(&C, op);
          }
        }
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