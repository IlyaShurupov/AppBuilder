
#pragma once

#include "Attribute.h"

using Args = Array<Value*>;
using ArgTypes = Array<ValType>;

struct Method {

    Dict<Value> args;
    Value out = Value(NONE);

    Obj* prnt;

    void (*method_body)(Method* This) = nullptr;

    Method(Obj* p_prnt);

    bool call();

    Method& operator = (void (*func)(Method* This)) {
        method_body = func;
        return *this;
    }
};