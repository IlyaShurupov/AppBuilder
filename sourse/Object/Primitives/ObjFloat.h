
#pragma once

#include "Object.h"

class Float : public Obj {
    
    float val = 0;

    Float& operator = (const Float& in);

    public:

    Float(const Float& in) : Obj(in) {
        val = in.val;
        max = in.max;  
        min = in.min;  
    }

    float max = FLT_MAX;
    float min = FLT_MIN;

    Float(Obj* prnt) : Obj (prnt)  {        
        RegisterType(ObjType("Float"));
    }

    virtual Float& Instance() {
        return *new Float(*this);
    }

    bool Assign(float _val, float _min, float _max) {
        if (!CanModify()) {
            return false;
        }
        val = _val;
        min = _min;
        max = _max;
        Modified(ModType::SET);
        return true;
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
        Modified(ModType::SET);
        return true;
    }

    bool Equal(const Obj& obj) {
        assert(type == obj.type);
        return val == ((Float&)obj).GetVal();
    }

    ~Float() {
    }
};