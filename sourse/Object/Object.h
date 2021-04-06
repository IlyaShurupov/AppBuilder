
#pragma once

#include "Types.h"

#define STRR const Str&
#define FOREACH_OBJ(Oblist, iter) FOREACH(Oblist.list, Obj, iter)

struct ObjType {

    ObjType();
    ObjType(STRR name);

    bool IsPrnt(STRR);

    Str idname;
    bool locked = true;
    ObjType* child = nullptr;
    ObjType* prnt = nullptr;
};

struct Obj {
    
    Obj();
    Obj(Obj* _prnt, const ObjType& _type);

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

    Obj& GetChld(STRR idname);
    Obj& Put(Obj* val, STRR idname);
    void Pop(STRR idname);

    Dict<Obj> props;
    ObjType type;
    Obj* prnt = nullptr;

    bool modified = false;
};

template <typename Class, typename PrntClass = Obj>
struct ObjBasedClass : PrntClass {

    void InitType() {
    
        ObjType* current_type = new ObjType();
        *current_type = PrntClass::type;
        current_type->child = &(PrntClass::type);
        PrntClass::type.prnt = current_type;

        PrntClass::type.idname = typeid(Class).name();
    }

    ObjBasedClass(Obj* _prnt) {
        PrntClass::prnt = _prnt;
        InitType();
    }

    ObjBasedClass() {
        InitType();
    }

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