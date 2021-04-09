
#pragma once

#include "Object.h"

class String : public ObjBasedClass<String> {
    
    friend ObjBasedClass;
    String() {}
    Str string;
    public:

    String(Obj* prnt) : ObjBasedClass (prnt) {}

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

    String& operator = (const String& in) {
        string = in.string;
        return *this;
    }

    ~String() {
    }

};