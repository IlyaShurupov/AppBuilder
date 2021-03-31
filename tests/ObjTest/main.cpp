
#include "Object.h"
#include "Types.h"

DEFINE_OBJ(Operator) {

    CONSTR(Operator) {

        ADD_METHOD(BOOL, Poll, ());
        ADD_METHOD(BOOL, Invoke, ());
        ADD_METHOD(BOOL, Modal, ());
    }

    
    DEF_METHOD(Poll) {
        GET_THIS(Operator);

    }

    DEF_METHOD(Invoke) {
        
    }

    DEF_METHOD(Modal) {
        
    }

};

DEFINE_OBJ(Thread) {

    CONSTR(Thread) {
        ADD_LINK(Operator, op);
    }
};

DEFINE_OBJ(UI) {

    CONSTR(UI) {

        ADD_METHOD(BOOL, PumpRequests, ());
        ADD_METHOD(BOOL, Output, ());
    }

    
    DEF_METHOD(PumpRequests) {
        GET_THIS(UI);

    }

    DEF_METHOD(Output) {
        GET_THIS(UI);

    }
};

DEFINE_OBJ(Programm) {

    CONSTR(Programm) {

        ADD_CHLD(Ops);
        ADD_CHLD(Threads);

        DECL_CHILD(UI, UI);
        
        ADD_CHLD(Data);

        ADD_METHOD(NONE, CoreLoop, ());

        ADD_INT(Fps) = (aligned)60;
    }

    DEF_METHOD(CoreLoop) {
        GET_THIS(Programm);

        USING_VAL(Data); 
        USING_VAL(Ops); 
        USING_VAL(UI);  
        USING_VAL(Threads);

        Timer timer = Timer();

        MAINLOOP : { // Main loop: Handle events -> run operators -> show result

            timer.duration = 1000.f / This->Get("Fps").Int();
            timer.reset();

            // Evaluate User's inputs
            UI.Link()->Call(nullptr, "PumpRequests", Args());

            // Run Operators from queue
            for (Iterator<Value> thread(&Threads.Link()->attributes, 0); thread.node();) {
                    
                // Go to the next thread
                ++thread;
            }

            /*
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

                
            }

            
            */

            UI.Link()->Call(nullptr, "Output", Args());

            if (!timer.timeout()) {
                TreadSleep(timer.remain());
            }
        }

        goto MAINLOOP;
    }

    bool run() {
        Value ret = Value(NONE, "status");
        Call(&ret, "CoreLoop", Args());
        return 0;
    }
};


int main () {
    COMPILE_OBJECT(Programm);  
    Programm->run();
}