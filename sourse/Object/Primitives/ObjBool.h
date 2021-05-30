
#pragma once

#include "Object.h"

class Bool : public Obj {
    
    bool val = 0;

    Bool& operator = (const Bool& in);

    public:

    Bool(const Bool& in) : Obj(in) {
        val = in.val;
    }

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
        Modified(ModType::SET);
        return true;
    }

    bool Equal(const Obj& obj) {
        assert(type == obj.type);
        return val == ((Bool&)obj).GetVal();
    }

    ~Bool() {
    }
};