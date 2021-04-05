#pragma once

#include "Object.h"
#include "Primitives.h"

struct Application : ObjBasedClass<Application> {
    
    Application(Obj* prnt) : ObjBasedClass (prnt) {
        ObList::Add(this, "UIs").type = "UI";
        ObList::Add(this, "Requests").type = "Request";
        ObList::Add(this, "Threads").type = "Thread";
        Int::Add(&Put(new Obj(this, "Data"), "Data"), "fps").Assign(60, 10, 120);
    }

    Application& operator = (const Application& in) {
        return *this;
    }

    void Run() {

        List<Obj>& UIs = ObList::Get(this, "UIs").list;
        List<Obj>& Threads = ObList::Get(this, "Threads").list;
        List<Obj>& Requests = ObList::Get(this, "Requests").list;


        MAINLOOP : {

            Timer timer = Timer(1000.f / Int::Get(&GetChld("Data"), "fps").val);

            // Pump Requests From UIs
            FOREACH(&UIs, Obj, ui) {
            }

            // Poll Requests
            FOREACH(&Requests, Obj, request) {
            }

            // Run Threads
            FOREACH(&Threads, Obj, thread) {
            }

            // Output to user
            FOREACH(&UIs, Obj, ui) {
            }

            timer.wait_out();

        } goto MAINLOOP;
    }

    ~Application() {
    }
};