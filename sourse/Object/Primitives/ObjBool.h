
#pragma once

#include "Object.h"

class Bool : public Obj {
    
    bool val = 0;

    Bool& operator = (const Bool& in);

    public:

    Bool(const Bool& in) : Obj(in) {
        val = in.val;
    }

    float max = FLT_MAX;
    float min = FLT_MIN;

    Bool(Obj* prnt) : Obj (prnt)  {        
        RegisterType(ObjType("Bool"));
    }

    virtual Bool& Instance() {
        return *new Bool(*this);
    }

    Bool& Assign(bool _val) {
        val = _val;
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