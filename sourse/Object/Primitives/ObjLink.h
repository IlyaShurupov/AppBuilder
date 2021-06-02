
#pragma once

#include "Object.h"

class Link : public Obj {
    
    Obj* link = nullptr;
    Str link_type = "Obj";
    Str current_type = "None";
    bool base_class = true;

    Link& operator = (const Link& in);

    public:

    Link(const Link& in) : Obj(in) {
        link_type = in.link_type;
        link = in.link;  
        base_class = in.base_class;  
        current_type = in.current_type;
    }

    Link(Obj* prnt) : Obj (prnt) {        
        RegisterType(ObjType("Link"));
    }

    virtual Link& Instance() {
        return *new Link(*this);
    }

    void Init(STRR _link_type, bool _base_class) {
        link_type = _link_type;
        base_class = _base_class;
    }

    bool SetLink(Obj* obj) {
        if (!CanModify()) {
            return false;
        }

        if (base_class) {
            if (obj->type.IsPrnt(link_type)) {
                link = obj;
                current_type = obj->type.idname;
            }
        } else if (obj->type.idname == link_type) {
            link = obj;
            current_type = obj->type.idname;
        } else {
            return false;
        }
        Modified(ModType::SET);
        return true;
    }

    bool Equal(const Obj& obj) {
        assert(type == obj.type);
        return link == ((Link&)obj).GetLink();
    }

    Obj* GetLink() {
        return link;
    }

    ~Link() {
    }
};