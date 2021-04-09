
#pragma once

#include "UI.h"
#include "Thread.h"

class ShortCut : public Requester {

    ShortCut& operator = (const ShortCut& in);
    ShortCut(const ShortCut& in) : Requester(in) {} 

    public:

    ShortCut(Obj* prnt) : Requester(prnt) {
        RegisterType(ObjType("ShortCut"));
    }

    virtual ShortCut& Instance() {
        return *new ShortCut(*this);
    }

    void ProcInputs(ObList* reqs) {
        CreateRequest(reqs);
    }
};

class KeyInput : public Obj {

    KeyInput& operator = (const KeyInput& in);
    KeyInput(const KeyInput& in) : Obj(in) {} 

    public:

    KeyInput(Obj* prnt) : Obj(prnt) {
        RegisterType(ObjType("KeyInput"));

        ADDOBJ(String, KeyName, *this, (this)).Assign("none");
        ADDOBJ(Int, ASCII Code, *this, (this)).Assign(0, 0, 255);
        ADDOBJ(Int, State, *this, (this)).Assign(0, 0, 5);
    }
    
    virtual KeyInput& Instance() {
        return *new KeyInput(*this);
    }

    void Update(Device* Device) {
        Int& state = GETOBJ(Int, this, State);
        int val = (int)Device->GetKeyState(GETOBJ(Int, this, ASCII Code).GetVal(), (InputState)state.GetVal());
        state.Set(val);
    }
};

class TUI : public UI {

    TUI& operator = (const TUI& in);
    TUI(const TUI& in) : UI(in) {
        dev = in.dev;
    } 

    public:

    TUI(Obj* prnt, Device* _dev) : UI(prnt) {
        ADDOBJ(ObList, Shortcuts, *this, (this)).Assign("ShortCut", false);
        ADDOBJ(ObList, Inputs, *this, (this)).Assign("KeyInput", false);
        dev = _dev;
    }
    
    virtual TUI& Instance() {
        return *new TUI(*this);
    }

    Device* dev = nullptr;
    
    void UpdateInputStates() {
        FOREACH_OBJ(&GETOBJ(ObList, this, Inputs).GetList(), input_obj) {
            KeyInput* input = (KeyInput*)input_obj.Data();
            input->Update(dev);
        }
    }

    void PumpRequests(ObList* requests) {
        UpdateInputStates();

        FOREACH_OBJ(&GETOBJ(ObList, this, Shortcuts).GetList(), shcut_obj) {
            ShortCut* shcut = (ShortCut*)shcut_obj.Data();
            shcut->ProcInputs(requests);
        }
    }

    ~TUI() {}
};