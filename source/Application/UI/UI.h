#pragma once

#include "Object.h"
#include "Primitives.h"

class UI : public Obj {

    UI& operator = (const UI& in);

    public:

    UI(const UI& in) : Obj(in) {} 

    UI(Obj* prnt) : Obj(prnt) {
        RegisterType("UI");
    }

    virtual UI& Instance() {
        return *new UI(*this);
    }

    virtual void PumpRequests(ObList* requests) {}
    virtual void OutPut(Obj* root) {}

    virtual ~UI() {}
};