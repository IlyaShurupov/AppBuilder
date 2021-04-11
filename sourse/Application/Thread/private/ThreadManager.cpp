
#include "Thread/ThreadManager.h"

OpHolder::OpHolder(const OpHolder& in) : Obj(in) {
    op = in.op;
    threads = in.threads;
} 


OpHolder::OpHolder(Obj* prnt, Operator* _op) : Obj(prnt) {
    RegisterType(ObjType("OpHolder"));
    op = _op;
}


Operator* OpHolder::GetInstance(ObDict* args_link, int* _instance_count) {
    UpdateThreads();

    if (op->parallel || !threads.Len()) {
        Operator* instance = &op->Instance();

        *_instance_count += 1;
        instance->instance_count = _instance_count;
        GETOBJ(Link, instance, Args).SetLink(args_link);

        threads.PushBack(instance);
        return instance;
    }

    return nullptr;
}

void OpHolder::GetInterface(ObDict* args) {
    delete args;
    args = &GETOBJ(ObDict, op, Interface).Instance();
}

void OpHolder::UpdateThreads() {
    FOREACH(&threads, Operator, op) {
        if (op->Finished()) {
            *op->instance_count -= 1;
            threads.DelNode(op.node());
        }
    }
}