
#pragma once

#include "Object.h"

struct ObDict : ObjBasedClass<ObDict> {
    
    Dict<Obj> dict;
    Str dict_type;
    bool base_class = false;

    ObDict(Obj* prnt) : ObjBasedClass (prnt) {}

    ObDict& operator = (const ObDict& in) {
        dict_type = in.dict_type;
        type = in.type;
        base_class = in.base_class;
        return *this;
    }

    ObDict& Assign(Str _dict_type, bool _base_class) {
        dict_type = _dict_type;
        base_class = _base_class;
        return *this;
    }

    bool AddObj(Obj* obj, STRR name) {
        if (base_class) {
            if (obj->type.IsPrnt(dict_type)) {
                dict.Put(name, obj);
                return true;
            }
            return true;
        } 
        if (obj->type.idname == dict_type) {
            dict.Put(name, obj);
            return true;
        }
        return false;
    }

    ~ObDict() {
    }
};