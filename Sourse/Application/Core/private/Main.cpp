
#include "Mem.h"
#include "Strings.h"
#include "Timer.h"
#include "Core/Operator.h"
#include "Core/Seance.h"
#include "UI/UInputs.h"
#include "Platform/SysHandler.h"
#include "UI/UInterface.h"

#define FPS 120.f

int main(int argc, char* argv[]) {

  Timer timer = Timer(1 * int(1000 / FPS));

  // Create Seance
  Seance& C = *NEW(Seance)(&Str(getExecutablePath()));

  // Main loop: Handle events -> run operators -> show result
  MAINLOOP : {

    timer.reset();

    // Evaluate User's inputs
    C.ui.Input(C);

    // Run Operators from queue
    for (Node<OpThread>* node = &C.threads.first(); node;) {

      OpThread* thread = node->Data;
      OpEvState* op_event = &thread->op_event;
      Operator* op = thread->op;

      switch (op->state) {

        case OpState::RUNNING_MODAL: // keep running    
          op->Modal(&C, node->Data->modalevent);
          break;

        case OpState::CANCELED:
          thread->state = ThreadState::CLOSED;
          op->state = OpState::NONE;
          // Todo: undo system
          break;

        case OpState::FINISHED:
          thread->state = ThreadState::CLOSED;
          op->state = OpState::NONE;
          break;

        case OpState::NONE: {

          if (!op->Poll(&C)) {
            thread->state = ThreadState::DENIED;
            op->state = OpState::NONE;
            break;
          }

          switch (*op_event) {

            case OpEvState::EXECUTE:
              op->Execute(&C);
              break;

            case OpEvState::INVOKE:
              op->Invoke(&C);
          }
        }
      }

      // Go to the next thread
      Node<OpThread>* del_node = node;
      node = node->Next;
      if (thread->state == ThreadState::DENIED || thread->state == ThreadState::CLOSED) {
        C.threads.delnode(del_node);
      }
    }

    C.ui.Output();

    if (!timer.timeout()) {
      TreadSleep(timer.remain());
    }

  } goto MAINLOOP;
}