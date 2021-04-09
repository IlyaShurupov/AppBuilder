
#pragma once

#include "Object.h"

class Float : public ObjBasedClass<Float> {
    
    friend ObjBasedClass;
    Float() {}
    float val = 0;
    public:

    float max = FLT_MAX;
    float min = FLT_MIN;

    Float(Obj* prnt) : ObjBasedClass (prnt) {}

    bool Assign(float _val, float _min, float _max) {
        if (!CanModify()) {
            return false;
        }
        val = _val;
        min = _min;
        max = _max;
        Modified();
        return true;
    }

    Float& operator = (const Float& in) {
        val = in.val;
        min = in.min;
        max = in.max;
        return *this;
    }
    
    float GetVal() {
        return val;
    }

    bool Set(float _val) {
        if (!CanModify()) {
            return false;
        }
        val = _val;
        CLAMP(val, min, max);
        Modified();
        return true;
    }

    ~Float() {
    }
};