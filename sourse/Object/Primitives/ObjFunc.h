
#pragma once

#include "Object.h"

struct Func : ObjBasedClass<Func> {
    
    Func(Obj* prnt) : ObjBasedClass (prnt) {}

    Func& operator = (const Func& in) {
        return *this;
    }

    virtual void call() {}

    virtual ~Func() {}
};