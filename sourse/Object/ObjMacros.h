#pragma once

// #include "Object.h"

#define USING_VAL(val_name) Value& val_name = This->Get(#val_name)
#define USING_VAL_AS(val_name, ref_name) Value& ref_name = This->Get(#val_name)

#define ADD_LINK(type, val_name) Value& val_name = Link(NAME(Obj_##type), #val_name)
#define ADD_BOOL(val_name) Value& val_name = Bool(#val_name)
#define ADD_INT(val_name) Value& val_name = Int(#val_name)
#define ADD_FLOAT(val_name) Value& val_name = Float(#val_name)

#define ADD_METHOD(ret, fname, args) Func(ret, #fname, ArgTypes args) = fname


#define CONSTR(class_name) Obj_##class_name(Obj* prnt) : Obj(prnt, #class_name)
            

#define DEFINE_OBJ(class_name) struct Obj_##class_name : Obj

#define DECL_CHILD(type_name, var_name) \
    Obj* chld; \
    if (!(chld = Create(#type_name, #var_name))) { \
        chld = &Define((Obj *) new Obj_##type_name(scope.Root()->parent)); } \
    Create(#type_name, #var_name)

#define CREATE(type_name, var_name) \
Obj& var_name = *Create(#type_name, #var_name) 

#define GET_THIS(obj_name) Obj_##obj_name* This = (Obj_##obj_name*)call->prnt;
#define DEF_METHOD(name) static void name(Method* call)

#define COMPILE_OBJECT(root_obj) Obj_##root_obj* root_obj = new Obj_##root_obj(nullptr)

#define ADD_CHLD(name) Obj& name = *AddChild(#name)