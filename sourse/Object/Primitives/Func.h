
#pragma once 

#include "Object.h"

#include "ObjMacros.h"
        

class Func : Obj {

    HEAD(Func);

    void (*func_body)() = nullptr;

    public:

    Func(Obj* p_prnt);
    Func& operator = (const Func& in);
    ~Func();

    void Set(void (*func_body)());
    void PassArg(Obj* arg, const Str& name);
    void Call();

    // ---- static --- //

    static Func& Add(Obj* to, const Str& idname);
    static Func& Get(Obj* from, const Str& idname);
};