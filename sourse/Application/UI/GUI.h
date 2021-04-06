#pragma once

#include "UI.h"

#include "Thread.h"

#include "Device/DevBuffer.h"

enum struct UIIstate {
  NONE = 0,
  ENTERED,
  INSIDE,
  LEAVED,
};


struct Guii : ObjBasedClass<Guii, Requester> {

    Guii() {}
    Guii(Obj* prnt) : ObjBasedClass (prnt) { Init (); }
    Guii(Obj* prnt, STRR) : ObjBasedClass (prnt) { Init (); }
    
    ObList* childs;

    void Init () {
        childs = &ObList::Add(this, "childs");
        childs->list_type = "Guii";
    }

    Guii& operator = (const Guii& in) { return *this; }

    // Draw info
    Rect<float> rect;
    DevBuffer* buff = nullptr;

    bool ownbuff = true;
    bool recursive_trunsform = false;
    vec2<bool> rigid;

    // Event info
    UIIstate state;
    bool hide = false;
    bool redraw = true;

    // Edit info
    Rect<float> prev_rect;
    char flag = 0;


    void OnUpdate(ObList* requests, ObList* inputs) {
        OnUpdateBody(requests, inputs);

        FOREACH(&childs->list, Obj, guii) {
            ((Guii*)guii.Data())->OnUpdate(requests, inputs);
        }
    }

    void OnDestroy(ObList* requests, ObList* inputs) {
        OnDestroyBody(requests, inputs);

        FOREACH(&childs->list, Obj, guii) {
            ((Guii*)guii.Data())->OnDestroy(requests, inputs);
        }
    }

    void OnCreate(ObList* requests, ObList* inputs) {
        OnCreateBody(requests, inputs);

        FOREACH(&childs->list, Obj, guii) {
            ((Guii*)guii.Data())->OnCreate(requests, inputs);
        }
    }

    void Draw(Obj* root) {
        DrawBody(root);
        
        FOREACH(&childs->list, Obj, guii) {
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

struct DataView : ObjBasedClass<DataView, Guii> {

    DataView(Obj* prnt) : ObjBasedClass (prnt) {}

    DataView& operator = (const DataView& in) { return *this; }
 
    void OnCreateBody(ObList* requests, ObList* inputs) {

    }

    void OnUpdateBody(ObList* requests, ObList* inputs) {

    }

    void OnDestroyBody(ObList* requests, ObList* inputs) {

    }

    void DrawBody(Obj* root) {

    }

    ~DataView() {}
};

struct GUI : ObjBasedClass<GUI, UI> {

    GUI(Obj* prnt) : ObjBasedClass (prnt) {
        Link::Add(this, "Inputs List").Init("ObList", false);
        Link::Add(this, "Root").Init("Guii", true);
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