
#pragma once


/*
#include "Object.h"

struct ObjList : Obj {

    List<Obj> list;
    Str type;

    ObjList(Obj* prnt, const Str& _type) : Obj("List", prnt) {
        type = _type;
        statd = (void *)this;
    }

    static void Add(Obj* obj, const Str& type, const Str& idname) {
        obj->props.Put(idname, (Obj*)new ObjList(obj, type));
    }

    static List<Obj>& Get(Obj* obj, const Str& idname) {
        Obj* list_obj = obj->props.Get(idname);
        assert(list_obj->type == Str("List"));
        return ((ObjList *)list_obj->statd)->list;
    }

    static Obj* PushBack(ObjList* ths, Obj* obj) {
        assert(ths->type == obj->type);
        ths->list.PushBack(obj);
        return obj;
    }
};

struct Int : Obj { 

    aligned min = ALIGNED_MIN;
    aligned max = ALIGNED_MAX;
    aligned val = 0;

    Int(Obj* prnt) : Obj("Int", prnt) {
        statd = (void *)this;
    }

    Int& Set(aligned _val) {
        CLAMP(_val, min, max);
        val = _val;
        return *this;
    }

    static Int& Add(Obj* to, const Str& idname) {
        Int* int_obj = new Int(to);
        to->props.Put(idname, (Obj*)int_obj);
        return *int_obj;
    }

    static Int& Get(Obj* from, const Str& idname) {
        Obj* int_obj = from->props.Get(idname);
        assert(int_obj->type == Str("Int"));
        return *(Int *)int_obj;
    }
};

struct ObjDict : Obj {

    Dict<Obj> dict;
    Str type;

    ObjDict(Obj* prnt, const Str& _type) : Obj("Dict", prnt) {
        type = _type;
        statd = (void *)this;
    }
};



struct Func : Obj {
    Func(Obj* prnt, void(*func)(Obj& ths)) : Obj("Func", prnt) {
        statd = (void *)func;
    }

    static void call(Obj& func_obj, const Str& idname) {
        Obj& functor = *func_obj.props.Get(idname);
        assert(functor.type == Str("Func"));
        ((void (*)(Obj& ths))functor.statd)(functor);
    }
};
*/