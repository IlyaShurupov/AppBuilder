
#pragma once

#include "Types.h"

struct StatD {
    void* data = nullptr;
    struct Obj* (*instantiate)(void* ths) = nullptr;
    void (*destruct)(void* data) = nullptr;
    void (*copy)(void* ths, void* in) = nullptr;
};

struct Obj {

    Str type;
    Obj* prnt = nullptr;
    Dict<Obj> props;
    StatD* statd = nullptr;
    
    Obj(const Str& _type, Obj* _prnt);

    Obj& operator = (const Obj& in);

    Obj& Add(const Str& idname);
    
    Obj& Get(const Str& idname);

    void Delete(const Str& idname);

    void Release(const Str& idname);

    virtual ~Obj();

    // ---- static --- //

    static Obj& Instantiate(const Obj& in);
};