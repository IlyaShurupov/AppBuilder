
#pragma once

#include "Object.h"

class Bool : public ObjBasedClass<Bool> {
    
    friend ObjBasedClass;
    Bool() {}
    bool val = 0;
    public:
    
    Bool(Obj* prnt) : ObjBasedClass (prnt) {}

    Bool& Assign(bool _val) {
        val = _val;
        return *this;
    }

    Bool& operator = (const Bool& in) {
        val = in.val;
        return *this;
    }

    bool GetVal() {
        return val;
    }

    bool Set(bool _val) {
        if (!CanModify()) {
            return false;
        }
        val = _val;
        Modified();
        return true;
    }

    ~Bool() {
    }
};