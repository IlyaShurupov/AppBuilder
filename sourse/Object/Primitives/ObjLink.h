
#pragma once

#include "Object.h"

class Link : public ObjBasedClass<Link> {
    
    Obj* link = nullptr;
    Str link_type;
    bool base_class = false;

    public:

    Link(Obj* prnt) : ObjBasedClass (prnt) {}

    Link& operator = (const Link& in) {
        link_type = in.link_type;
        link = in.link;
        base_class = in.base_class;
        return *this;
    }

    void Init(STRR _link_type, bool _base_class) {
        link_type = _link_type;
        base_class = _base_class;
    }

    bool SetLink(Obj* obj) {
        if (base_class) {
            if (obj->type.IsPrnt(link_type)) {
                link = obj;
                return true;
            }
            return true;
        } 
        if (obj->type.idname == link_type) {
            link = obj;
            return true;
        }
        return false;
    }

    Obj* GetLink() {
        return link;
    }

    ~Link() {
    }
};