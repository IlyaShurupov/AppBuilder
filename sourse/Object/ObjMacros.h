
#pragma once

#define REGISTER statd = new StatD(); \
    statd->data = this; \
    statd->instantiate = instantiate; \
    statd->destruct = destruct; \
    statd->copy = copy;

#define HEAD(ObClass) ; \
    static Obj* instantiate (void* ths) {  \
        ObClass* me = (ObClass*)ths; \
        ObClass* new_obj = new ObClass(me->prnt, me); \
        *new_obj = *me; \
        return (Obj*)new_obj; \
    }; \
    static void destruct (void* data) { delete (ObClass*)data; }; \
    static void copy(void* ths, void* in) { (*(ObClass*)ths) = (*(ObClass*)in); }; \
    ObClass(Obj* prnt, ObClass* me) : Obj(#ObClass, prnt) { REGISTER; }  \

#define UNREGISTER delete statd; statd = nullptr;
