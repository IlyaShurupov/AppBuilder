
#include "Operator/Operator.h"
#include "Core/Seance.h"
#include "Memory/Mem.h"
#include "Time/Timer.h"
#include "Strings.h"
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
    for (Iterator<OpThread> thread(&C.threads, 0); thread < C.threads.Len();) {

      OpEvState* op_event = &thread->op_event;
      Operator* op = thread->op;

      switch (op->state) {

        case OpState::RUNNING_MODAL: // keep running    
          op->Modal(&C, thread->modalevent);
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

      if (thread->state == ThreadState::DENIED || thread->state == ThreadState::CLOSED) {
        C.threads.Detach(thread.Node());
      }

      // Go to the next thread
      ++thread;
    }

    C.ui.Output();

    if (!timer.timeout()) {
      TreadSleep(timer.remain());
    }

  } goto MAINLOOP;
}