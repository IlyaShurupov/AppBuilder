
#pragma once

#include "Object.h"

#include "ObjMacros.h"
        

class Link : Obj {

    HEAD(Link);

    Str obj_type;
    Obj* target = nullptr;

    public:

    Link(Obj* p_prnt, const Str& _type);
    Link& operator = (const Link& in);
    ~Link();

    Link* ChangeType(const Str& type);
    Link* Set(Obj* _target);

    // ---- static --- //

    static Link* Add(Obj* prnt, const Str& type, const Str& idname);
    static Link* Get(Obj* prnt, const Str& idname);

};