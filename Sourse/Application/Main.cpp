
#include "public/Seance.h"

#define FPS 120.f

void test();

int main(int argc, char* argv[]) {

  // enable memory debuging
#ifdef _DEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  test();

  {
    // Create Seance
    Seance& C = *DBG_NEW Seance(&getExecutablePath());

    Timer timer = Timer(1 * int(1000 / FPS));

    // Main loop: Handle events -> run operators -> show result
    while (true) {

      timer.reset();

      // Handle events for each window
      FOREACH_NODE(Window, (&C.project.windows), win_node) {
        Window* win = win_node->Data;
        win->ProcessEvents(&C.op_threads);
      }

      // Run Operators from queu (This is where the fun happends)
      // TODO: prioritize commands by sorting them in the list

      for (Node<OpThread>* node = &C.op_threads.first(); node;) {

        OpThread* thread = node->Data;
        OpEventState* op_event = &thread->op_event;
        Operator* op = thread->op;

        switch (op->state) {

          case OpState::RUNNING_MODAL:
            // keep running
            op->Modal(&C, op, node->Data->modal_event);
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
            if (!op->Poll(&C, op)) {
              thread->state = ThreadState::DENIED;
              op->state = OpState::NONE;
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
        // Go to the next thread
        Node<OpThread>* del_node = node;
        node = node->Next;
        if (thread->state == ThreadState::DENIED || thread->state == ThreadState::CLOSED) {
          C.op_threads.del(del_node);
        }
      }

      if (C.op_threads.len()) {
        // Draw each window & show updates
        FOREACH_NODE(Window, (&C.project.windows), win_node) {
          Window* win = win_node->Data;
          win->Draw();
          win->SendBuffToSystem();
        }
      }

      if (!timer.timeout()) {
        THREAD_SLEEP(timer.remain());
      }
    }
  }

  // Print out mem leaks
#ifdef _DEBUG
  _CrtDumpMemoryLeaks();
#endif
}

void test() {
  Object root;
  root.Pos.assign(1, 1, 1);

  Object ch11, ch13;
  Object ch111, ch112;

  root.hrchy_dbg.add(&ch13);
  
  ch13.hrchy_dbg.join(&root);

  ch112.hrchy_dbg.join(&root);
  ch111.hrchy_dbg.join(&ch112);

  Object* rootptr = ch111.hrchy_dbg.root();
  rootptr->Pos.assign(0, 0, 0);
  
  ch111.hrchy_dbg.leave();

}