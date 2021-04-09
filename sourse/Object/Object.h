
#pragma once

#include "Types.h"

#define STRR const Str&
#define FOREACH_OBJ(Oblist, iter) FOREACH(Oblist, Obj, iter)
#define ADDOBJ(Type, Name, to, Args) ((Type &)(to).AddChld(new Type Args, #Name))

class Obj;

struct OnModCallBack {
    OnModCallBack(Obj* _ths, void (*_callback)(Obj* ths)) {
        callback = _callback;
        ths = _ths;
    }
    Obj* ths;
    void (*callback)(Obj* ths);
};

struct ObjType {

    ObjType();
    ObjType(STRR name);

    bool IsPrnt(STRR);

    Str idname;
    bool locked = false;
    ObjType* child = nullptr;
    ObjType* prnt = nullptr;
};

class Obj {
    
    public:
    
    Obj();
    Obj(Obj* _prnt, const ObjType& _type);

    virtual void Copy(Obj* in) {
        props = in->props;
        type = in->type;
        // prnt = in->prnt;
        Modified();
    }

    virtual Obj& Instance() {
        Obj* instance = new Obj(prnt, type);
        *instance = *this;
        return *instance;
    }

    virtual ~Obj() {}

    Obj& GetChld(STRR idname);
    Obj& AddChld(Obj* chld, STRR idname);
    void DelChild(STRR idname);

    Dict<Obj> props;
    ObjType type;
    Obj* prnt = nullptr;

    // modification callbacks

    Obj* req_mod_param = nullptr;
    bool (*req_mod_poll)(Obj* req_mod_param) = nullptr;

    void BindModPoll(Obj* ths, bool (*call)(Obj* ths)) {
        req_mod_poll = call;
        req_mod_param = ths;
    }

    bool CanModify() {
        if (type.locked) {
            return false;
        }
        if (req_mod_poll) {
            return req_mod_poll(req_mod_param);
        }
        return true;
    }

    Array<OnModCallBack> OnModCallBacks;

    void AddOnModCallBack(Obj* ths, void (*call)(Obj* ths)) {
        OnModCallBacks.PushBack(OnModCallBack(ths, call));
    }

    void Modified() {
        for (int i = 0; i < OnModCallBacks.Len(); i++) {
            OnModCallBacks[i].callback(OnModCallBacks[i].ths);
        }
    }
};

template <typename Class, typename PrntClass = Obj>
class ObjBasedClass : public PrntClass {

    public:

    void InitType() {
    
        ObjType* current_type = new ObjType();
        *current_type = PrntClass::type;
        current_type->child = &(PrntClass::type);
        PrntClass::type.prnt = current_type;

        PrntClass::type.idname = (typeid(Class).name() + 1);
    }

    ObjBasedClass(Obj* _prnt) {
        PrntClass::prnt = _prnt;
        InitType();
    }

    ObjBasedClass() {
        InitType();
    }

    virtual void Copy(Obj* in) {
        *((Class*)this) = *((Class*)in);
        PrntClass::Copy(in);
    }

    virtual Obj& Instance() {
        Obj* instance = new Class();
        instance->Copy(this);
        return *instance;
    }

    virtual ~ObjBasedClass() {}

    static Class& Get(Obj* from, STRR idname) {
        return (Class&)*from->props.Get(idname);
    }

};