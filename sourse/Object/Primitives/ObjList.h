
#pragma once

#include "Object.h"

struct ObList : ObjBasedClass<ObList> {
    
    List<Obj> list;
    Str type;

    ObList(Obj* prnt) : ObjBasedClass (prnt) {}

    ObList& operator = (const ObList& in) {
        list = in.list;
        type = in.type;
        return *this;
    }

    ~ObList() {
    }
};