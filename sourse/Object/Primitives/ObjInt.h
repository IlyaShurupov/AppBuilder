
#pragma once

#include "Object.h"

class Int : public ObjBasedClass<Int> {
    
    friend ObjBasedClass;
    Int(){}
    aligned val = 0;
    public:

    aligned max = ALIGNED_MAX;
    aligned min = ALIGNED_MIN;
    
    Int(Obj* prnt) : ObjBasedClass (prnt) {}

    bool Assign(aligned _val, aligned _min, aligned _max) {
        if (!CanModify()) {
            return false;
        }
        val = _val;
        min = _min;
        max = _max;
        Modified();
        return true;
    }

    Int& operator = (const Int& in) {
        val = in.val;
        min = in.min;
        max = in.max;
        return *this;
    }
    
    aligned GetVal() {
        return val;
    }

    bool Set(aligned _val) {
        if (!CanModify()) {
            return false;
        }
        val = _val;
        CLAMP(val, min, max);
        Modified();
        return true;
    }

    ~Int() {
    }
};
