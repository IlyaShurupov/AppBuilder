#pragma once

#include "Object.h"
#include "Primitives.h"

enum class OpState {
    NONE,
    INVOKED,
    RUNNING,
    FINISHED,
};

class Operator : public Obj {

    Operator& operator = (const Operator& in);
    public:

    Operator(const Operator& in);

    int* instance_count = nullptr;
    bool parallel = false; 
    OpState state = OpState::NONE;

    Operator(Obj* prnt);

    virtual Operator& Instance() {
        return *new Operator(*this);
    }

    virtual bool Poll() { return false; }
    virtual void Invoke() {}
    virtual void Modal() {}

    virtual ~Operator() {}

    bool Finished();
};