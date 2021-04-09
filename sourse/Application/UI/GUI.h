#pragma once

#include "UI.h"

#include "Thread.h"

struct Guii : ObjBasedClass<Guii, Requester> {

    Guii() {}
    Guii(Obj* prnt) : ObjBasedClass (prnt) { Init (); }
    Guii(Obj* prnt, STRR) : ObjBasedClass (prnt) { Init (); }
    
    ObList* childs;

    void Init () {
        childs = &ADDOBJ(ObList, childs, *this, (this));
        childs->Assign("Guii", true);
    }

    Guii& operator = (const Guii& in) { return *this; }

    void OnUpdate(ObList* requests, ObList* inputs) {
        OnUpdateBody(requests, inputs);

        FOREACH_OBJ(&childs->GetList(), guii) {
            ((Guii*)guii.Data())->OnUpdate(requests, inputs);
        }
    }

    void OnDestroy(ObList* requests, ObList* inputs) {
        OnDestroyBody(requests, inputs);

        FOREACH_OBJ(&childs->GetList(), guii) {
            ((Guii*)guii.Data())->OnDestroy(requests, inputs);
        }
    }

    void OnCreate(ObList* requests, ObList* inputs) {
        OnCreateBody(requests, inputs);

        FOREACH_OBJ(&childs->GetList(), guii) {
            ((Guii*)guii.Data())->OnCreate(requests, inputs);
        }
    }

    void Draw(Obj* root) {
        DrawBody(root);
        
        FOREACH_OBJ(&childs->GetList(), guii) {
            ((Guii*)guii.Data())->Draw(root);
        }
    }

    void Transform() {}
    
    virtual void OnUpdateBody(ObList* requests, ObList* inputs) {}
    virtual void OnDestroyBody(ObList* requests, ObList* inputs) {}
    virtual void OnCreateBody(ObList* requests, ObList* inputs) {}
    virtual void DrawBody(Obj* root) {}

    virtual ~Guii() {}
};

struct GUI : ObjBasedClass<GUI, UI> {

    GUI() {}
    GUI(Obj* prnt) : ObjBasedClass (prnt) {
        ADDOBJ(Link, Inputs List, *this, (this)).Init("ObList", false);
        ADDOBJ(Link, Root, *this, (this)).Init("Guii", true);
    }
    
    GUI& operator = (const GUI& in) { return *this; }
    
    void PumpRequests(ObList* requests) {
        ObList* inputs = (ObList*)Link::Get(this, "Inputs List").GetLink();
        Guii* root = (Guii*)Link::Get(this, "Root").GetLink();
        if (inputs && root) {
            root->OnUpdate(requests, inputs);
        }
    }

    void OutPut(Obj* root) {
        Guii* guii_root = (Guii*)Link::Get(this, "Root").GetLink();
        if (root && guii_root) {
            guii_root->Draw(root);
        }
    }

    ~GUI() {}
};