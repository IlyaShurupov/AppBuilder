
#pragma once

#include "Object.h"

struct Link : ObjBasedClass<Link> {
    
    Obj* link = nullptr;
    Str type;

    Link(Obj* prnt) : ObjBasedClass (prnt) {}

    Link& operator = (const Link& in) {
        type = in.type;
        link = in.link;
        return *this;
    }

    void Set(Obj* obj) {
        assert(obj->type == type);
        link = obj;
    }

    ~Link() {
    }
};