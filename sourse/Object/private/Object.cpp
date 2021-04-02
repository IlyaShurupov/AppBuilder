
#include "Object.h"

Obj* Obj::Root() { 
    if (parent) {
        return parent->Root(); 
    }
    return this;
}

Obj::Obj(Obj* prnt, const Str& p_type) {
    type = p_type;
    parent = prnt;
}    


// ---------------------------------------------------- //

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

Obj* Obj::AddChild(const Str& idname) {
    Value* newcld = new Value(LINK);
    *newcld = new Obj(this, idname);
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

// ---------------------------------------------------- //


Obj* Obj::FindTemplate(const Str& type) { 
    
    Obj* def = nullptr;
    if (templates.Get(type, &def)) {
        return def;
    }

    if (parent) {
        return parent->FindTemplate(type);
    } 

    return nullptr;
}

Obj& Obj::SaveAsTemplate() {
    Obj* obj = new Obj(nullptr, type);
    *obj = *this;
    obj->parent = nullptr;
    templates.Put(obj->type, obj);
    return *obj;
}

Obj* Obj::InstantiateTemplateAsChild(const Str& type, const Str& idname) {
    
    Obj* templt = FindTemplate(type);

    assert(templt);

    Obj* newobj = new Obj(this, idname);
    *newobj = *templt;
    newobj->parent = this;
    
    Value* newcld = new Value(LINK);
    *newcld = newobj;

    attributes.Put(idname, newcld);

    return newobj;
}

// ---------------------------------------------------- //


Method& Obj::AddFunc(ValType ret_type, const Str& idname, const ArgTypes& type_args) {
    Method* method = new Method(this);
    methods.Put(idname, method);
    return *method;
}

void Obj::Call(const Str& idname) {
    Method* mthd = nullptr;

    if (!methods.Get(idname, &mthd)) {
        assert(mthd);
    }

    mthd->call_method();
}