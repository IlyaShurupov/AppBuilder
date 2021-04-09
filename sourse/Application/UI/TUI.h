
#pragma once

#include "UI.h"
#include "Thread.h"

struct ShortCut : public ObjBasedClass<ShortCut, Requester> {

    ShortCut() {}
    ShortCut(Obj* prnt) : ObjBasedClass(prnt) {
        
    }
    
    void ProcInputs(ObList* reqs) {
        CreateRequest(reqs);
    }
};

struct KeyInput : public ObjBasedClass<KeyInput> {
    KeyInput() {}
    KeyInput(Obj* prnt) :ObjBasedClass(prnt) {
        ADDOBJ(String, KeyName, *this, (this)).Assign("none");
        ADDOBJ(Int, ASCII Code, *this, (this)).Assign(0, 0, 255);
        ADDOBJ(Int, State, *this, (this)).Assign(0, 0, 5);
    }

    void Update(Device* Device) {
        Int& state = Int::Get(this, "State");
        int val = (int)Device->GetKeyState(Int::Get(this, "ASCII Code").GetVal(), (InputState)state.GetVal());
        state.Set(val);
    }
};

struct TUI : ObjBasedClass<TUI, UI> {

    TUI() {}
    TUI(Obj* prnt, Device* _dev) : ObjBasedClass (prnt) {
        ADDOBJ(ObList, Shortcuts, *this, (this)).Assign("ShortCut", false);
        ADDOBJ(ObList, Inputs, *this, (this)).Assign("KeyInput", false);
        dev = _dev;
    }
    
    Device* dev = nullptr;
    TUI& operator = (const TUI& in) { return *this; }
    
    void UpdateInputStates() {
        FOREACH_OBJ(&ObList::Get(this, "Inputs").GetList(), input_obj) {
            KeyInput* input = (KeyInput*)input_obj.Data();
            input->Update(dev);
        }
    }

    void PumpRequests(ObList* requests) {
        UpdateInputStates();

        FOREACH_OBJ(&ObList::Get(this, "Shortcuts").GetList(), shcut_obj) {
            ShortCut* shcut = (ShortCut*)shcut_obj.Data();
            shcut->ProcInputs(requests);
        }
    }

    ~TUI() {}
};