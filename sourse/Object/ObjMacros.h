#pragma once

// #include "Object.h"

#define USING_VAL(val_name) Value& val_name = This->Get(#val_name)
#define USING_VAL_AS(val_name, ref_name) Value& ref_name = This->Get(#val_name)

#define ADD_LINK(type, val_name) Value& val_name = AddLink(NAME(Obj_##type), #val_name)
#define ADD_BOOL(val_name) Value& val_name = AddBool(#val_name)
#define ADD_INT(val_name) Value& val_name = AddInt(#val_name)
#define ADD_FLOAT(val_name) Value& val_name = AddFloat(#val_name)
#define ADD_STRING(val_name) Value& val_name = AddString(#val_name)

#define ADD_METHOD(ret, fname, args) AddFunc(ret, #fname, ArgTypes args) = fname
#define VIRTUAL(ret, fname, args) AddFunc(ret, #fname, ArgTypes args)


#define COMPILE(class_name) Obj_##class_name(Obj* prnt) : Obj(prnt, #class_name)
            

#define DEFINE_OBJ(class_name) struct Obj_##class_name : Obj



#define CREATE(type_name, var_name) \
Obj& var_name = *Create(#type_name, #var_name) 

#define GET_THIS(obj_name) Obj_##obj_name* This = (Obj_##obj_name*)call->prnt;
#define DEF_METHOD(name) static void name(Method* call)

#define COMPILE_OBJECT(root_obj) Obj_##root_obj* root_obj = new Obj_##root_obj(nullptr)

#define ADD_CHLD(name) Obj& name = *AddChild(#name)