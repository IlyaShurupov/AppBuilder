#pragma once 

#include "Types.h"

struct Obj;

enum ValType {
    NONE, 
    BOOL, 
    INT, 
    FLOAT,
    LINK,
};

struct Value {

    Str val_idname;
    ValType type;
    aligned bytes;
    Str lnk_type;

    Value(ValType p_type, const Str& p_val_idname);  

    Value& operator = (const Value& in);
    Value& operator = (const aligned& in);
    Value& operator = (const float& in);
    Value& operator = (const bool& in);
    Value& operator = (Obj* in);

    bool IsVal() { return type != NONE; };
    Obj* Link() { return (Obj *)bytes; };
    aligned Int() { return bytes; };
    float Float() { return (float)bytes; } ;
    bool Bool() { return (bool)bytes; };
};

using Args = Array<Value*>;
using ArgTypes = Array<ValType>;

struct Method {

    Str method_idname;
    ArgTypes arg_types;
    ValType type_out;

    const Args* arguments;
    Value* out;
    void (*method_call)(Method* This) = nullptr;

    Obj* prnt;

    Method(Obj* p_prnt, ValType p_out, const Str& p_idname, const ArgTypes& type_args);

    Method& operator = (void (*func)(Method* This)) {
        method_call = func;
        return *this;
    }
};

struct Scope {

    List<Obj> objs;
    List<Obj> defenitions;
    Obj* parent;

    Scope* Root();
    Obj* FindDefenition(const Str& type_nmae);
};

struct Obj {

    Str obj_idname;
    Str type_idname;
    Scope scope;
    List<Method> methods;
    List<Value> attributes;

    Obj(Obj* prnt, const Str& p_type_idname);   

    Obj& Define(Obj* obj);
    Obj* Create(const Str& type_idname, const Str& name);

    Value& Val(const Str& idname);
    Value& Link(const Str& obj_type, const Str& idname);
    Value& Int(const Str& idname);
    Value& Float(const Str& idname);
    Value& Bool(const Str& idname);

    Method& Func(ValType ret_type, const Str& idname, const ArgTypes& type_args);

    Value& Get(const Str& idname);
    void Call(Value* out, const Str& idname, const Args& arguments);

    Obj& operator = (const Obj& in);
};