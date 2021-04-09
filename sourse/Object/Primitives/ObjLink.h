
#pragma once

#include "Object.h"

class Link : public ObjBasedClass<Link> {
    
    friend ObjBasedClass;
    Link() {}
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
        if (!CanModify()) {
            return false;
        }

        if (base_class) {
            if (obj->type.IsPrnt(link_type)) {
                link = obj;
            }
        } else if (obj->type.idname == link_type) {
            link = obj;
        } else {
            return false;
        }
        Modified();
        return true;
    }

    Obj* GetLink() {
        return link;
    }

    ~Link() {
    }
};