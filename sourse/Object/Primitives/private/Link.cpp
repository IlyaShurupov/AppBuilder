#include "Primitives/Link.h"


Link::Link(Obj* p_prnt, const Str& _type) : Obj("Link", p_prnt) { 
    REGISTER;
    obj_type = _type;
}

Link::~Link() {
    UNREGISTER;
}

Link& Link::operator = (const Link& in) {
    obj_type = in.obj_type;
    target = in.target;
    return *this;
}


Link* Link::ChangeType(const Str& type) {
    target = 0;
    obj_type = type;
    return this;
}

Link* Link::Set(Obj* _target) {
    assert(obj_type == _target->type);
    target = _target;
    return this;
}


Link* Link::Add(Obj* prnt, const Str& type, const Str& idname) {
    Link* link = new Link(prnt, type);
    prnt->props.Put(idname, (Obj*)link);
    return link;
}

Link* Link::Get(Obj* prnt, const Str& idname) {
    Obj* link_obj = prnt->props.Get(idname);
    assert(link_obj->type == Str("Link"));
    return (Link*)link_obj;
}