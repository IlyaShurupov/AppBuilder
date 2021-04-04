
#include "Object.h"

Obj::Obj(const Str& _type, Obj* _prnt) {
    prnt = _prnt;
    type = _type;
}

Obj::~Obj() {
    if (statd) {
        statd->destruct(statd->data);
    }
}

Obj& Obj::operator = (const Obj& in) {
    assert((type == in.type) && ((bool)in.statd == (bool)statd));

    if (statd) {
        statd->copy(statd->data, in.statd->data);
    }
    return *this;
}

Obj& Obj::Add(const Str& idname) {
    Obj* obj = new Obj(idname, this);
    props.Put(idname, obj);
    return *obj;
}


Obj& Obj::Get(const Str& idname) {
    return *props.Get(idname);
}

void Obj::Delete(const Str& idname) {
    Obj* del = props.Get(idname);
    delete del;
    props.Remove(idname);
}

void Obj::Release(const Str& idname) {
    props.Remove(idname);
}

Obj& Obj::Instantiate(const Obj& in) {
    if (in.statd) {
        return *in.statd->instantiate(in.statd->data);
    }
    Obj* new_obj = new Obj(in.type, in.prnt);
    return *new_obj = in;
}