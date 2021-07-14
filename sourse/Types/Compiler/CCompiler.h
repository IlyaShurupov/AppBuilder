#pragma once

#include "Strings/Strings.h"


class DynFunc {

    void* state = nullptr;
    
    void init();
    void free();

public:

    Str program;

    DynFunc();
    bool compile();
    void* get_func(const Str& symbol);
    ~DynFunc();
};