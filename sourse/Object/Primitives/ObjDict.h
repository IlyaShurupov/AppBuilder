
#pragma once

#include "Object.h"

class ObDict : public Obj {
    
    DictObj dict;
    Str dict_type;
    bool base_class = false;

    ObDict& operator = (const ObDict& in);

    public:

    ObDict(const ObDict& in) : Obj(in) {
        dict_type = in.dict_type;
        dict = in.dict;  
        base_class = in.base_class;  
    }

    ObDict(Obj* prnt) : Obj (prnt)  {        
        RegisterType(ObjType("ObDict"));
    }

    virtual ObDict& Instance() {
        return *new ObDict(*this);
    }

    ObDict& Assign(Str _dict_type, bool _base_class) {
        dict_type = _dict_type;
        base_class = _base_class;
        return *this;
    }

    DictObj& GetDict() {
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
        
        Modified(ModType::SET);
        return true;
    }

    Obj& GetObj(STRR name) {
        return *dict.Get(name);
    }

    ~ObDict() {
    }
};