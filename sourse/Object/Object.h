#pragma once 

#include "Attribute.h"
#include "Method.h"

#include "ObjMacros.h"

struct Scope {
    Dict<Obj> objs;
    Dict<Obj> defenitions;
    Obj* parent;

    Scope* Root();
    Obj* FindDefenition(const Str& type);
};

struct Obj {

    Str type;
    
    Scope scope;

    Dict<Method> methods;
    Dict<Value> attributes;

    Obj(Obj* prnt, const Str& p_type);   

    Value& AddVal(const Str& idname);
    Value& AddLink(const Str& obj_type, const Str& idname);
    Value& AddInt(const Str& idname);
    Value& AddFloat(const Str& idname);
    Value& AddBool(const Str& idname);
    Value& AddString(const Str& idname);

    Method& AddFunc(ValType ret_type, const Str& idname, const ArgTypes& type_args);

    Obj& Define(Obj* obj);
    Obj* AddChild(const Str& idname);
    Obj* InstantiateAsChild(const Obj* defenition, const Str& idname);

    Obj* RTCreate(const Str& type, const Str& name);

    Value& Get(const Str& idname);
    void Call(Value* out, const Str& idname, const Args& arguments);
};