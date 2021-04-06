#pragma once

#include "Object.h"
#include "Primitives.h"

#include "UI/GUI.h"
#include "UI/TUI.h"

struct Application : ObjBasedClass<Application> {
    
    Application(Obj* prnt) : ObjBasedClass (prnt) {
        
        ObList::Add(this, "UIs").Assign("UI", true);
        ObList::Add(this, "Requests").Assign("Operator", true);
        ObList::Add(this, "Threads").Assign("Operator", true);
        ObList::Add(this, "Operators").list_type = "Operator";

        Int::Add(&Put(new Obj(this, ObjType("Data")), "Data"), "fps").Assign(60, 10, 120);
    }

    Application& operator = (const Application& in) {
        return *this;
    }

    void Compose() {
        ObList::Get(this, "UIs").list.PushBack(new GUI(this));
        ObList::Get(this, "UIs").list.PushBack(new TUI(this));
        // Guii& guii_root = Guii::Add(GUI, "Root");
        // ObList::Get(&guii_root, "childs").list.PushBack(new DataView(&guii_root));
    }

    void Run() {

        ObList& UIs = ObList::Get(this, "UIs");
        ObList& Threads = ObList::Get(this, "Threads");
        ObList& Requests = ObList::Get(this, "Requests");

        MAINLOOP : {

            Timer timer = Timer(1000.f / Int::Get(&GetChld("Data"), "fps").val);

            // Pump Requests From UIs
            FOREACH(&UIs.list, Obj, ui) {
                ((UI*)ui.Data())->PumpRequests(&Requests);
            }

            // Poll Requests
            FOREACH(&Requests.list, Obj, request) {
                Operator* op = (Operator*)request.Data();
                if (op->Poll()) {
                    Requests.list.Detach(request.node());
                    Threads.list.PushBack(request.node());
                    op->state = "INVOKED";
                }
            }

            // Run Threads
            FOREACH(&Threads.list, Obj, thread) {
                Operator* op = (Operator*)thread.Data();

                if (op->state == "INVOKED") {
                    op->Invoke();
                } 

                if (op->state == "RUNNING") {
                    op->Modal();
                } 
                
                if (op->state == "FINISHED") {
                    Threads.list.Detach(thread.node());
                }
            }   

            // Output to user
            FOREACH(&UIs.list, Obj, ui) {
                ((UI*)ui.Data())->OutPut(prnt);
            }

            timer.wait_out();

        } goto MAINLOOP;
    }

    ~Application() {
    }
};