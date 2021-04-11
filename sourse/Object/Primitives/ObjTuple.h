
#pragma once

#include "Object.h"

#include "ObjLink.h"

class ObjTuple : public Obj {
    
    ObjTuple& operator=(const ObjTuple& in);

    public:

    ObjTuple(const ObjTuple& in) : Obj(in) {}

    ObjTuple(Obj* prnt) : Obj(prnt) {
        RegisterType(ObjType("Tuple"));
        ADDOBJ(Link, Head, *this, (this));
        ADDOBJ(Link, Tail, *this, (this));
    }

    virtual ObjTuple& Instance() {
        return *new ObjTuple(*this);
    }

    Link& GetHead() {
        return GETOBJ(Link, this, Head);
    }

    Link& GetTail() {
        return GETOBJ(Link, this, Tail);
    }
    
    ~ObjTuple() {
    }
};
