
#include "Primitives/Func.h"

Func::Func(Obj* p_prnt) : Obj("Func", p_prnt) {
    REGISTER;
}

Func& Func::operator = (const Func& in) {
    func_body = in.func_body;
    return *this;
}

Func::~Func() {
    UNREGISTER;
}

void Func::Set(void (*p_func_body)()) {
    func_body = p_func_body;
}

void Func::PassArg(Obj* arg, const Str& name) {
    props.Put(name, arg);
}

void Func::Call() {
    assert(func_body);
    func_body();
}

// ------------- //

Func& Func::Add(Obj* to, const Str& idname) {
    Func* func = new Func(to);
    to->props.Put(idname, (Obj*)func);
    return *func;
}

Func& Func::Get(Obj* from, const Str& idname) {
    Obj* func_obj = from->props.Get(idname);
    assert(func_obj->type == Str("Func"));
    return *(Func*)func_obj;
}
