
#include "Object.h"

Scope* Scope::Root() { 
    if (parent) {
        return parent->scope.Root(); 
    }
    return this;
}

Obj* Scope::FindDefenition(const Str& type) { 
    
    Obj* def = nullptr;
    if (objs.Get(type, &def)) {
        return def;
    }

    if (parent) {
        return parent->scope.FindDefenition(type);
    } 

    return nullptr;
}


Obj::Obj(Obj* prnt, const Str& p_type) {
    type = p_type;
    scope.parent = prnt;
}    


Value& Obj::AddVal(const Str& idname) {
    Value* val = new Value(NONE);
    attributes.Put(idname, val);
    return *val;
}

Value& Obj::AddLink(const Str& obj_type, const Str& idname) {
    Value* newval = new Value(LINK);
    newval->data.link_data.link_type = obj_type;
    attributes.Put(idname, newval);
    return *newval;
}

Value& Obj::AddFloat(const Str& idname) {
    Value* val = new Value(FLOAT);
    attributes.Put(idname, val);
    return *val;
}

Value& Obj::AddInt(const Str& idname) {
    Value* val = new Value(INT);
    attributes.Put(idname, val);
    return *val;
}

Value& Obj::AddBool(const Str& idname) {
    Value* val = new Value(BOOL);
    attributes.Put(idname, val);
    return *val;
}

Value& Obj::AddString(const Str& idname) {
    Value* val = new Value(STRING);
    attributes.Put(idname, val);
    return *val;
}

Method& Obj::AddFunc(ValType ret_type, const Str& idname, const ArgTypes& type_args) {
    Method* method = new Method(this, ret_type, type_args);
    methods.Put(idname, method);
    return *method;
}

Obj& Obj::Define(Obj* obj) {
    scope.defenitions.Put(obj->type, obj);
    return *obj;
}

Obj* Obj::RTCreate(const Str& p_type, const Str& name) {
    Obj* def = scope.FindDefenition(type);

    if (def) {
        Obj* newobj = new Obj(this, p_type); 
        *newobj = *def;
        scope.objs.Put(name, newobj);
        return newobj;
    }

    return nullptr;
}

Obj* Obj::AddChild(const Str& idname) {
    Value* newcld = new Value(LINK);
    *newcld = new Obj(this, idname);
    attributes.Put(idname, newcld);
    return newcld->AsLink();
}

Obj* Obj::InstantiateAsChild(const Obj* defenition, const Str& idname) {
    Value* newcld = new Value(LINK);
    Obj* newobj = new Obj(this, idname);
    *newobj = *defenition;
    *newcld = newobj;
    attributes.Put(idname, newcld);
    return newcld->AsLink();
}

Value& Obj::Get(const Str& idname) {
    Value* val = nullptr;
    if (attributes.Get(idname, &val)) {
        return *val;
    }
    assert(0);
}

void Obj::Call(Value* out, const Str& idname, const Args& arguments) {
    Method* mthd = nullptr;

    if (!methods.Get(idname, &mthd)) {
        assert(mthd);
    }

    mthd->arguments = &arguments;
    mthd->out = out;

    // check args, ret type
    mthd->method_call(mthd);
}