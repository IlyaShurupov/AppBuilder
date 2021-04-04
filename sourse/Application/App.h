#pragma once

#include "Types.h"
#include "Object.h"

/*
struct Application : Obj {
    Application() : Obj("Application", nullptr) {
        
        ObjList::Add(this, "UI", "UIs");
        ObjList::Add(this, "Threads", "Thread");
        ObjList::Add(this, "Operators", "Operator");
        ObjList::Add(this, "Requests", "Request");

        Add("Data");

        props.Put("fps", (Obj*)new Int(this));

        props.Put("Run", (Obj*)new Func(this, Run));
    }

    static void Run(Obj& call) {
        Application* app = (Application *)(call.prnt); 

        List<Obj>& UIs = ObjList::Get(app, "UIs");
        List<Obj>& Threads = ObjList::Get(app,"Threads");
        List<Obj>& Requests = ObjList::Get(app,"Requests");


        MAINLOOP : {

            Timer timer = Timer(1000.f / (aligned)app->props.Get("fps")->statd);

            // Pump Requests From UIs
            FOREACH(&UIs, Obj, ui) {
                ui->props.Get("PumpRequests");
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
};
*/