#pragma once 

#include "Attribute.h"
#include "Method.h"

#include "ObjMacros.h"


struct Obj {

    Obj* parent;
    
    Obj(Obj* prnt, const Str& p_type);   
    Obj* Root();


    // ------------------------ //

    Dict<Value> attributes;

    Value& AddVal(const Str& idname);
    Value& AddLink(const Str& obj_type, const Str& idname);
    Value& AddInt(const Str& idname);
    Value& AddFloat(const Str& idname);
    Value& AddBool(const Str& idname);
    Value& AddString(const Str& idname);
    Obj* AddChild(const Str& idname);
    Value& Get(const Str& idname);


    // ------------------------ //

    Dict<Method> methods;

    Method& AddFunc(ValType ret_type, const Str& idname, const ArgTypes& type_args);
    void Call(const Str& idname);

    // ------------------------ //

    Str type;
    Dict<Obj> templates;

    Obj* FindTemplate(const Str& type);
    Obj& SaveAsTemplate();
    Obj* InstantiateTemplateAsChild(const Str& type, const Str& idname);

};