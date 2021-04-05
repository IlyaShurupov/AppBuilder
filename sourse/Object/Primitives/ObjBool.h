
#pragma once

#include "Object.h"

struct Bool : ObjBasedClass<Bool> {
    
    bool val = 0;

    Bool(Obj* prnt) : ObjBasedClass (prnt) {}

    Bool& Assign(bool _val) {
        val = _val;
        return *this;
    }

    Bool& operator = (const Bool& in) {
        val = in.val;
        return *this;
    }

    void Set(bool _val) {
        val = _val;
    }

    ~Bool() {
    }
};