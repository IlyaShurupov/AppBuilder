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
    Value& AddValLink(const Str& idname);
    Value& AddInt(const Str& idname);
    Value& AddFloat(const Str& idname);
    Value& AddBool(const Str& idname);
    Value& AddList(const Str& idname);
    Value& AddString(const Str& idname);
    Obj* AddChild(const Str& idname);

    Value& Get(const Str& idname);
    aligned& GetInt(const Str& idname);
    float& GetFloat(const Str& idname);
    bool& GetBool(const Str& idname);
    ValList& GetList(const Str& idname);
    Str& GetString(const Str& idname);
    Obj& GetLink(const Str& idname);
    Value& GetValLink(const Str& idname);

    // ------------------------ //

    Dict<Method> methods;

    Method& AddMethod(const Str& idname);
    Method& GetMethod(const Str& idname);
    void Call(const Str& idname);

    // ------------------------ //

    Str type;
    Dict<Obj> templates;

    Obj* FindTemplate(const Str& type);
    Obj& SaveAsTemplate();
    Obj* InstantiateTemplateAsChild(const Str& type, const Str& idname);

};