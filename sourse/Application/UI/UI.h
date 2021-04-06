#pragma once

#include "Object.h"
#include "Primitives.h"

struct UI : ObjBasedClass<UI> {

    UI() {}
    UI(Obj* prnt) : ObjBasedClass (prnt) {}
    UI(Obj* prnt, STRR) : ObjBasedClass (prnt) {}
    
    UI& operator = (const UI& in) { return *this; }
    
    virtual void PumpRequests(ObList* requests) {}
    virtual void OutPut(Obj* root) {}

    virtual ~UI() {}
};