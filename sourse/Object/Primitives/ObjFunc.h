
#pragma once

#include "Object.h"

class Func : public ObjBasedClass<Func> {
    
    public:
    
    Func() {}
    Func(Obj* prnt) : ObjBasedClass (prnt) {}

    Func& operator = (const Func& in) {
        return *this;
    }

    virtual void call() {}

    virtual ~Func() {}
};