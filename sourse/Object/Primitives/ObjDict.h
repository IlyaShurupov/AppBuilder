
#pragma once

#include "Object.h"

class ObDict : public ObjBasedClass<ObDict> {
    
    friend ObjBasedClass;
    ObDict() {}

    Dict<Obj> dict;
    Str dict_type;
    bool base_class = false;
    public:
    
    ObDict(Obj* prnt) : ObjBasedClass (prnt) {}

    ObDict& operator = (const ObDict& in) {
        dict = in.dict;
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

    Dict<Obj>& GetDict() {
        return dict;
    }

    bool AddObj(Obj* obj, STRR name) {
        if (!CanModify()) {
            return false;
        }

        if (base_class) {
            if (obj->type.IsPrnt(dict_type)) {
                dict.Put(name, obj);
            }
        } else if (obj->type.idname == dict_type) {
            dict.Put(name, obj);
        } else {
            return false;
        }
        
        Modified();
        return true;
    }

    ~ObDict() {
    }
};