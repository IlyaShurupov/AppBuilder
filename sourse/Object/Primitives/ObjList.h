
#pragma once

#include "Object.h"

struct ObList : ObjBasedClass<ObList> {
    
    List<Obj> list;
    Str list_type;
    bool base_class = false;

    ObList(Obj* prnt) : ObjBasedClass (prnt) {}

    ObList& operator = (const ObList& in) {
        list_type = in.list_type;
        type = in.type;
        return *this;
    }

    ObList& Assign(Str _list_type, bool _base_class) {
        list_type = _list_type;
        base_class = _base_class;
        return *this;
    }

    bool AddObj(Obj* obj) {
        if (base_class) {
            if (obj->type.IsPrnt(list_type)) {
                list.PushBack(obj);
                return true;
            }
            return true;
        } 
        if (obj->type.idname == list_type) {
            list.PushBack(obj);
            return true;
        }
        return false;
    }

    ~ObList() {
    }
};