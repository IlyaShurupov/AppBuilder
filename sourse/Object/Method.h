
#pragma once

#include "Attribute.h"

using Args = Array<Value*>;
using ArgTypes = Array<ValType>;

struct Method {

    ArgTypes arg_types;
    ValType type_out;

    const Args* arguments;
    Value* out;
    void (*method_call)(Method* This) = nullptr;

    Obj* prnt;

    Method(Obj* p_prnt, ValType p_out, const ArgTypes& type_args);

    Method& operator = (void (*func)(Method* This)) {
        method_call = func;
        return *this;
    }
};