
#pragma once

#include "Types.h"

#define STRR const Str&
#define FOREACH_OBJ(Oblist, iter) FOREACH(Oblist, Obj, iter)
#define ADDOBJ(Type, Name, to, Args) ((Type &)(to).AddChld(new Type Args, #Name))
#define GETOBJ(type, from, name) ((type&)*(from)->props.Get(#name))


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
    
    Obj& operator=(const Obj& in);
    
    public:
    
    Obj(const Obj& in);
    Obj(Obj* _prnt);

    virtual Obj& Instance() {
        return *new Obj(*this);
    }

    virtual ~Obj() {}

    Obj& GetChld(STRR idname);
    Obj& AddChld(Obj* chld, STRR idname);
    void DelChild(STRR idname);
    void RegisterType(const ObjType& _type);
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