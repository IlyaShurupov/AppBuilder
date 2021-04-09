
#pragma once

#include "Object.h"

class String : public Obj {
    
    Str string;
    
    String& operator=(const String& in);

    public:

    String(const String& in) : Obj(in) {
        string = in.string;    
    }

    String(Obj* prnt) : Obj(prnt) {
        RegisterType(ObjType("String"));
    }

    virtual String& Instance() {
        return *new String(*this);
    }

    bool Assign(STRR _string) {
        if (!CanModify()) {
            return false;
        }
        string = _string;
        Modified();
        return true;
    }

    const Str& GetStr() {
        return string;
    }

    ~String() {
    }
};
