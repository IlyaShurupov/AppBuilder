
#pragma once

#include "UI.h"
#include "Thread.h"

struct ShortCut : public ObjBasedClass<ShortCut, Requester> {
    ShortCut() {}
    void ProcInputs() {

    }
};

struct KeyInput : public ObjBasedClass<KeyInput> {
    KeyInput() {
        String::Add(this, "KeyName").Assign("none");
        Int::Add(this, "ASCII Code").Assign(0, 0, 255);
        Int::Add(this, "State").Assign(0, 0, 5);
    }

    void Update(Device* Device) {
        int val = Device->GetKeyState(Int::Get(this, "ASCII Code").val);
        Int::Get(this, "State").Set(val);
    }
};

struct TUI : ObjBasedClass<TUI, UI> {

    TUI(Obj* prnt) : ObjBasedClass (prnt) {
        ObList::Add(this, "Shortcuts").Assign("ShortCut", false);
        ObList::Add(this, "Inputs").Assign("KeyInput", false);
    }
    
    TUI& operator = (const TUI& in) { return *this; }
    
    void UpdateInputStates() {
        FOREACH_OBJ(&ObList::Get(this, "Inputs"), shcut_obj) {
            ShortCut* shcut = (ShortCut*)shcut_obj.Data();
            shcut->ProcInputs();
        }
    }

    void PumpRequests(ObList* requests) {
        UpdateInputStates();

        FOREACH_OBJ(&ObList::Get(this, "Shortcuts"), shcut_obj) {
            ShortCut* shcut = (ShortCut*)shcut_obj.Data();
            shcut->ProcInputs();
        }
    }

    ~TUI() {}
};