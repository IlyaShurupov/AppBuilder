
#include "Object.h"

bool Value::IsVal() { return type != NONE; };
Obj* Value::Link() { return (Obj *)bytes; };
aligned Value::Int() { return bytes; };
float Value::Float() { return IntToFlt(bytes); } ;
bool Value::Bool() { return (bool)bytes; };

Value::Value(ValType p_type, const Str& p_val_idname) {
    type = p_type;
    val_idname = p_val_idname;
}

Value& Value::operator = (const Value& in) {
    type = in.type;
    bytes = in.bytes;
    return *this;
}

Value& Value::operator = (const aligned& in) {
    type = INT;
    bytes = in;
    return *this;
}

Value& Value::operator = (const float& in) {
    type = FLOAT;
    bytes = FltToInt(in);
    return *this;
}

Value& Value::operator = (const bool& in) {
    type = BOOL;
    bytes = (aligned)in;
    return *this;
}

Value& Value::operator = (Obj* in) {
    type = LINK;
    bytes = (aligned)in;
    return *this;
}


Method::Method(Obj* p_prnt, ValType p_out, const Str& p_idname, const ArgTypes& p_arg_types) {
    prnt = p_prnt;
    type_out = p_out;
    method_idname = p_idname;
    arg_types = ArgTypes(p_arg_types);
}


Obj::Obj(Obj* prnt, const Str& p_type_idname) {
    type_idname = p_type_idname;
    scope.parent = prnt;
}    

Obj& Obj::Define(Obj* obj) {
    scope.defenitions.PushBack(obj);
    return *obj;
}

Scope* Scope::Root() { 
    if (parent) {
        return parent->scope.Root(); 
    }
    return this;
}

Obj* Scope::FindDefenition(const Str& type_name) { 
    FOREACH(&defenitions, Obj, i) {
        if (i->type_idname == type_name) {
            return i.Data();
        }
    }

    if (parent) {
        return parent->scope.FindDefenition(type_name);
    } 

    return nullptr;
}

Obj* Obj::Create(const Str& type_idname, const Str& name) {
    Obj* def = scope.FindDefenition(type_idname);

    if (def) {
        Obj* newobj = new Obj(this, type_idname); 
        *newobj = *def;
        newobj->obj_idname = name;
        return newobj;
    }

    return nullptr;
}

Value& Obj::Val(const Str& idname) {
    Value* val = new Value(NONE, idname);
    attributes.PushBack(val);
    return *val;
}

Value& Obj::Link(const Str& obj_type, const Str& idname) {
    Value* newval = new Value(LINK, idname);
    newval->lnk_type = obj_type;
    attributes.PushBack(newval);
    return *newval;
}

Value& Obj::Float(const Str& idname) {
    Value* val = new Value(FLOAT, idname);
    attributes.PushBack(val);
    return *val;
}

Value& Obj::Int(const Str& idname) {
    Value* val = new Value(INT, idname);
    attributes.PushBack(val);
    return *val;
}

Value& Obj::Bool(const Str& idname) {
    Value* val = new Value(BOOL, idname);
    attributes.PushBack(val);
    return *val;
}

Method& Obj::Func(ValType ret_type, const Str& idname, const ArgTypes& type_args) {
    Method* method = new Method(this, ret_type, idname, type_args);
    methods.PushBack(method);
    return *method;
}

Value& Obj::Get(const Str& idname) {
    FOREACH(&attributes, Value, i) {
        if (i->val_idname == idname) {
            return *i.Data();
        }
    } 
    assert(0);
}

void Obj::Call(Value* out, const Str& idname, const Args& arguments) {
    Method* mthd = nullptr;
    FOREACH(&methods, Method, i) {
        if (i->method_idname == idname) {
            mthd = i.Data();
            break;
        }
    } 
    
    assert(mthd);

    mthd->arguments = &arguments;
    mthd->out = out;

    // check args, ret type
    mthd->method_call(mthd);
}


Obj& Obj::operator = (const Obj& in) {
    return *this;
}