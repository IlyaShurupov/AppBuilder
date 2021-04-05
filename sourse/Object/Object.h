
#pragma once

#include "Types.h"

#define STRR const Str&

struct Obj {
    
    Obj(Obj* _prnt, STRR _type) {
        type = _type; 
        prnt = _prnt;
    }

    virtual void Copy(Obj* in) {
        props = in->props;
        type = in->type;
        prnt = in->prnt;
    }

    virtual Obj& Instance() {
        Obj* instance = new Obj(prnt, type);
        *instance = *this;
        return *instance;
    }

    virtual ~Obj() {}

    Obj& GetChld(STRR idname) {
        return *props.Get(idname);
    }

    Obj& Put(Obj* val, STRR idname) {
        props.Put(idname, val);
        return *val;
    }

    void Pop(STRR idname) {
        props.Remove(idname);
    }

    Dict<Obj> props;
    Str type;
    Obj* prnt = nullptr;

};

template <typename Class, typename PrntClass = Obj>
struct ObjBasedClass : PrntClass {

    ObjBasedClass(Obj* prnt) : PrntClass(prnt, typeid(Class).name()) {}

    virtual void Copy(Obj* in) {
        *(Class*)this = *(Class*)in;
        PrntClass::Copy(in);
    }

    virtual Obj& Instance() {
        Obj* instance = new Class(this->prnt);
        instance->Copy(this);
        return *instance;
    }

    virtual ~ObjBasedClass() {}

    static Class& Add(Obj* to, STRR idname) {
        Obj* newobj = new Class(to);
        to->props.Put(idname, newobj);
        return *(Class*)newobj;
    }

    static Class& Get(Obj* from, STRR idname) {
        return (Class&)*from->props.Get(idname);
    }

};