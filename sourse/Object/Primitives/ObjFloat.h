
#pragma once

#include "Object.h"

struct Float : ObjBasedClass<Float> {
    
    float val = 0;
    float max = FLT_MAX;
    float min = FLT_MIN;

    Float(Obj* prnt) : ObjBasedClass (prnt) {}

    Float& Assign(float _val, float _min, float _max) {
        val = _val;
        min = _min;
        max = _max;
        return *this;
    }

    Float& operator = (const Float& in) {
        val = in.val;
        min = in.min;
        max = in.max;
        return *this;
    }

    void Set(float _val) {
        val = _val;
        CLAMP(val, min, max);
    }

    ~Float() {
    }
};