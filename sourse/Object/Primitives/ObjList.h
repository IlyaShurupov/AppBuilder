
#pragma once

#include "Object.h"

class ObList : public Obj {
    
    List<Obj> list;
    Str list_type;
    bool base_class = false;

    ObList& operator = (const ObList& in);
    
    public:

    ObList(const ObList& in) : Obj(in) {
        list_type = in.list_type;
        type = in.type;  
    }

    ObList(Obj* prnt) : Obj(prnt) {
        RegisterType(ObjType("ObList"));
    }

    virtual ObList& Instance() {
        return *new ObList(*this);
    }

    ObList& Assign(Str _list_type, bool _base_class) {
        list_type = _list_type;
        base_class = _base_class;
        return *this;
    }

    List<Obj>& GetList() {
        return list;
    }

    bool AddObj(Obj* obj) {
        if (!CanModify()) {
            return false;
        }

        if (base_class) {
            if (obj->type.IsPrnt(list_type)) {
                list.PushBack(obj);
            }
        } else if (obj->type.idname == list_type) {
            list.PushBack(obj);
        } else {
            return false;
        }

        Modified(ModType::SET);
        return true;
    }

    ~ObList() {
    }
};