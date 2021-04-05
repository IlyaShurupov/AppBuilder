
#pragma once

#include "Object.h"

struct String : ObjBasedClass<String> {
    
    Str string;

    String(Obj* prnt) : ObjBasedClass (prnt) {}

    String& Assign(STRR _string) {
        string = _string;
        return *this;
    }

    String& operator = (const String& in) {
        string = in.string;
        return *this;
    }

    ~String() {
    }
};