
#pragma once

#include "Object.h"

struct Int : ObjBasedClass<Int> {
    
    aligned val = 0;
    aligned max = ALIGNED_MAX;
    aligned min = ALIGNED_MIN;

    Int(Obj* prnt) : ObjBasedClass (prnt) {}

    Int& Assign(aligned _val, aligned _min, aligned _max) {
        val = _val;
        min = _min;
        max = _max;
        return *this;
    }

    Int& operator = (const Int& in) {
        val = in.val;
        min = in.min;
        max = in.max;
        return *this;
    }

    void Set(aligned _val) {
        val = _val;
        CLAMP(val, min, max);
    }

    ~Int() {
    }
};
