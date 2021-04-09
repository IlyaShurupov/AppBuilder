#pragma once

#include "Object.h"
#include "Primitives.h"

#include "UI/GUI.h"
#include "UI/TUI.h"

#include <stdlib.h>


class QuitProgramm : public Operator {

    QuitProgramm& operator = (const QuitProgramm& in);
    QuitProgramm(const QuitProgramm& in) : Operator(in.prnt) {} 

    public:

    QuitProgramm(Obj* prnt) : Operator(prnt) { 
        ObDict& args = GETOBJ(ObDict, this, Interface);
        args.AddObj(new Bool(nullptr), "Save");
    }

    virtual QuitProgramm& Instance() {
        return *new QuitProgramm(*this);
    }
    
    bool Poll() { return true; }
    
    void Invoke() { 
        state = OpState::RUNNING; 
    }
    
    void Modal() { 
        Dict<Obj>* args = &((ObDict*)GETOBJ(Link, this, Args).GetLink())->GetDict();
        bool save = ((Bool *)args->Get("Save"))->GetVal();
        // exit(0);
        state = OpState::FINISHED; 
    }
};



class Application : public Obj {
    
    Application& operator = (const Application& in);
    Application(const Application& in) : Obj(in) {

    } 

    public:

    Application(Obj* prnt) : Obj (prnt) {
        
        RegisterType(ObjType("Application"));

        ADDOBJ(ObList, UIs, *this, (this)).Assign("UI", true);
        ADDOBJ(ObList, Requests, *this, (this)).Assign("Operator", true);
        ADDOBJ(ObList, Threads, *this, (this)).Assign("Operator", true);
        ADDOBJ(ObList, OpHolders, *this, (this)).Assign("OpHolder", false);

        Obj& data = ADDOBJ(Obj, Data, *this, (this));
        ADDOBJ(Int, fps, data, (&data)).Assign(60, 10, 120);
    }

    void Compose() {
        Device* dev = new Device();

        ObList& OpHolders = GETOBJ(ObList, this, OpHolders);
        Operator* op = new QuitProgramm(nullptr);
        op->Invoke();
        OpHolder* opholder = new OpHolder(nullptr, op);
        OpHolders.AddObj(opholder);

        TUI* tui = new TUI(this, dev);
        GETOBJ(ObList, this, UIs).AddObj(tui);

        KeyInput* input = new KeyInput(nullptr);
        GETOBJ(String, input, KeyName).Assign("A");
        GETOBJ(Int, input, ASCII Code).Set('A');
        GETOBJ(ObList, tui, Inputs).AddObj(input);

        ShortCut* shcut = new ShortCut(nullptr);
        GETOBJ(Link, shcut, Target Op).SetLink(opholder);
        GETOBJ(ObList, tui, Shortcuts).AddObj(shcut);
    }

    void Run() {

        ObList& UIs = GETOBJ(ObList, this, UIs);
        ObList& Threads = GETOBJ(ObList, this, Threads);
        ObList& Requests = GETOBJ(ObList, this, Requests);

        MAINLOOP : {

            Timer timer = Timer(1000.f / GETOBJ(Int, &GetChld("Data"), fps).GetVal());

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