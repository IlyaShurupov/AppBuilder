
#include "Object.h"

Obj* objs_entry = nullptr;

Obj* CopyObj::operator()(Obj* in) {
        return &in->Instance();
}

ObjType::ObjType() {
    idname = "Obj";
}

ObjType::ObjType(STRR name) {
    idname = name;
}

bool ObjType::IsPrnt(STRR prnt_id) {
    ObjType* type_node = this;
    while (type_node) {
        if (type_node->idname == prnt_id) {
            return true;
        }
        type_node = type_node->prnt;
    }
    return false;
}


Obj::Obj(const Obj& in) {
    props = in.props;

    type = in.type;
    prnt = in.prnt;

    req_mod_param = in.req_mod_param;
    req_mod_poll = in.req_mod_poll;
    OnModCallBacks = in.OnModCallBacks;

    if (objs_entry) {
        objs_entry->next = this;
        prev = objs_entry;
    }

    objs_entry = this;
}

Obj::Obj(Obj* _prnt) {
    type = ObjType("Obj");
    prnt = _prnt;

    if (objs_entry) {
        objs_entry->next = this;
        prev = objs_entry;
    }

    objs_entry = this;
}

void Obj::RegisterType(const ObjType& _type) {
    ObjType* current_type = new ObjType();
    *current_type = type;
    current_type->child = &(type);

    type = _type;
    type.prnt = current_type;
}

Obj& Obj::GetChld(STRR idname) {
    return *props.Get(idname);
}

Obj& Obj::AddChld(Obj* val, STRR idname) {
    props.Put(idname, val);
    return *val;
}

void Obj::DelChild(STRR idname) {
    props.Remove(idname);
}

Obj* get_objs_entry() {
    return objs_entry;
}