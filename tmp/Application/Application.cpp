

#include "Application.h"

#include "Operator/Operator.h"
#include "Data/Seance.h"
#include "Memory/Mem.h"
#include "Time/Timer.h"
#include "Strings/Strings.h"
#include "UI/UInputs.h"
#include "UI/UInterface.h"

void Application::LoadConfiguration(Str* projpath, UIItem* (*UIIFromStr)(Str* id, Operators* ops, DataBlock* paramsdb, DataBlock* uiidb)) {
  C->OnRead(projpath, UIIFromStr);
}


Application::Application(uint2 p_fps) {
  timer = Timer(1 * int(1000 / p_fps));
  C = NEW(Seance);
}

void Application::Launch() {

  if (!C) {
    return;
  }

  MAINLOOP : { // Main loop: Handle events -> run operators -> show result

    timer.reset();

    // Evaluate User's inputs
    C->ui.Input(*C);

    // Run Operators from queue
    for (Iterator<OpThread> thread(&C->threads, 0); thread.node();) {

      OpEvState* op_event = &thread->op_event;
      Operator* op = thread->op;

      switch (op->state) {

        case OpState::RUNNING_MODAL:
          op->Modal(C, thread->modalevent);
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

          if (!op->Poll(C)) {
            thread->state = ThreadState::DENIED;
            op->state = OpState::NONE;
            break;
          }

          switch (*op_event) {

            case OpEvState::EXECUTE:
              op->Execute(C);
              break;

            case OpEvState::INVOKE:
              op->Invoke(C);
          }
        }
      }

      if (thread.Data()->state == ThreadState::DENIED || thread.Data()->state == ThreadState::CLOSED) {
        C->threads.Detach(thread.node());
      }

      // Go to the next thread
      ++thread;
    }

    C->ui.Output();

    if (!timer.timeout()) {
      TreadSleep(timer.remain());
    }
  }
  goto MAINLOOP;
}

void Application::AddOperator(Operator* op) {
  C->ops.ops.PushBack(op);
}