
#pragma once

#include "Object.h"

struct ObDict : ObjBasedClass<ObDict> {
    
    Dict<Obj> dict;
    Str type;

    ObDict(Obj* prnt) : ObjBasedClass (prnt) {}

    ObDict& operator = (const ObDict& in) {
        dict = in.dict;
        type = in.type;
        return *this;
    }

    ~ObDict() {
    }
};