#pragma once

#include "Object.h"
#include "Primitives.h"

#include "Thread.h"

class OpHolder : public Obj {

    OpHolder& operator = (const OpHolder& in);
    OpHolder(const OpHolder& in);

    public:

    Operator* op;
    List<Operator> threads;

    OpHolder(Obj* prnt, Operator* _op);
    
    Operator* GetInstance(ObDict* args_link, int* _instance_count); 
    void GetInterface(ObDict* args);
    void UpdateThreads();

    virtual OpHolder& Instance() {
        return *new OpHolder(*this);
    }
};