#pragma once

#include "Object.h"
#include "Primitives.h"

#include "UI/GUI.h"
#include "UI/TUI.h"

#include <stdlib.h>

struct DataView : ObjBasedClass<DataView, Guii> {

    DataView() {}
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


class QuitProgramm : public ObjBasedClass<QuitProgramm, Operator> {

    public:
    QuitProgramm() {
        ObDict& args = ObDict::Get(this, "Interface");
        args.AddObj(new Bool(nullptr), "Save");
    }
    
    bool Poll() { return true; }
    
    void Invoke() { 
        state = OpState::RUNNING; 
    }
    
    void Modal() { 
        Dict<Obj>* args = &((ObDict*)Link::Get(this, "Args").GetLink())->GetDict();
        bool save = ((Bool *)args->Get("Save"))->GetVal();
        // exit(0);
        state = OpState::FINISHED; 
    }
};



struct Application : ObjBasedClass<Application> {
    
    Application() {}
    Application(Obj* prnt) : ObjBasedClass (prnt) {
        
        ADDOBJ(ObList, UIs, *this, (this)).Assign("UI", true);
        ADDOBJ(ObList, Requests, *this, (this)).Assign("Operator", true);
        ADDOBJ(ObList, Threads, *this, (this)).Assign("Operator", true);
        ADDOBJ(ObList, OpHolders, *this, (this)).Assign("OpHolder", false);

        Obj& data = ADDOBJ(Obj, Data, *this, (this, ObjType("Data")));
        ADDOBJ(Int, fps, data, (&data)).Assign(60, 10, 120);
    }

    Application& operator = (const Application& in) {
        return *this;
    }

    void Compose() {
        Device* dev = new Device();

        ObList& OpHolders = ObList::Get(this, "OpHolders");
        Operator* op = new QuitProgramm();
        op->Invoke();
        OpHolder* opholder = new OpHolder(nullptr, op);
        OpHolders.AddObj(opholder);

        TUI* tui = new TUI(this, dev);
        ObList::Get(this, "UIs").AddObj(tui);

        KeyInput* input = new KeyInput(nullptr);
        String::Get(input, "KeyName").Assign("A");
        Int::Get(input, "ASCII Code").Set('A');
        ObList::Get(tui, "Inputs").AddObj(input);

        ShortCut* shcut = new ShortCut(nullptr);
        Link::Get(shcut, "Target Op").SetLink(opholder);
        ObList::Get(tui, "Shortcuts").AddObj(shcut);
    }

    void Run() {

        ObList& UIs = ObList::Get(this, "UIs");
        ObList& Threads = ObList::Get(this, "Threads");
        ObList& Requests = ObList::Get(this, "Requests");

        MAINLOOP : {

            Timer timer = Timer(1000.f / Int::Get(&GetChld("Data"), "fps").GetVal());

            // Pump Requests From UIs
            FOREACH_OBJ(&UIs.GetList(), ui) {
                ((UI*)ui.Data())->PumpRequests(&Requests);
            }

            // Poll Requests
            FOREACH_OBJ(&Requests.GetList(), request) {
                Operator* op = (Operator*)request.Data();
                if (op->Poll()) {
                    Requests.GetList().Detach(request.node());
                    Threads.GetList().PushBack(request.node());

                    op->Invoke();
                }
            }

            // Run Threads
            FOREACH_OBJ(&Threads.GetList(), thread) {
                Operator* op = (Operator*)thread.Data();

                op->Modal();
                
                if (op->Finished()) {
                    Threads.GetList().Detach(thread.node());
                }
            }   

            // Output to user
            FOREACH_OBJ(&UIs.GetList(), ui) {
                ((UI*)ui.Data())->OutPut(prnt);
            }

            timer.wait_out();

        } goto MAINLOOP;
    }

    ~Application() {
    }
};