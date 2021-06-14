
#pragma once

#include "Object.h"

class Int : public Obj {
    
    aligned val = 0;
    
    Int& operator = (const Int& in);

    public:

    Int(const Int& in) : Obj(in) {
        val = in.val;
        max = in.max;  
        min = in.min;  
    }

    aligned max = ALIGNED_MAX;
    aligned min = ALIGNED_MIN;

    Int(Obj* prnt) : Obj (prnt) {        
        RegisterType(ObjType("Int"));
    }

    virtual Int& Instance() {
        return *new Int(*this);
    }

    bool Assign(aligned _val, aligned _min, aligned _max) {
        if (!CanModify()) {
            return false;
        }
        val = _val;
        min = _min;
        max = _max;
        Modified(ModType::SET);
        return true;
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
        Modified(ModType::SET);
        return true;
    }

    bool Equal(const Obj& obj) {
        assert(type == obj.type);
        return val == ((Int&)obj).GetVal();
    }

    void as_string(Str* str) {
      *str = Str((int)val);
    }

    bool from_string(Str* str) {
      int input_val = 0;
      if (str->to_val(input_val) && Set(input_val)) {
        return true;
      }
      return false;
    }

    ~Int() {
    }
};
