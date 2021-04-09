
#include "Object.h"

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


Obj::Obj() {}

Obj::Obj(Obj* _prnt, const ObjType& _type) {
    type = _type; 
    prnt = _prnt;
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
