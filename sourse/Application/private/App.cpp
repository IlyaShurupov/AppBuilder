
#include "App.h"


Application::Application() : Obj(nullptr, "Programm") {

    AddList("UIs");
    AddList("Requests");
    AddList("Threads");

    AddChild("Data");

    AddList("Operators");
    AddInt("fps") = (aligned)60;

    AddMethod("Run") = CoreLoop;
    SaveAsTemplate();
}

void Application::CoreLoop(Method* cd) {
    
    Application& ths = *(Application *)cd->prnt; 

    ValList& UIs = ths.GetList("UIs");
    ValList& Requests = ths.GetList("Requests");
    ValList& Threads = ths.GetList("Threads");
    
    MAINLOOP : { // Main loop: Handle events -> run operators -> show result

        Timer timer = Timer(1000.f / ths.GetInt("fps"));

        // Pump Requests From UIs
        FOREACH(&UIs, Value, ui) {
            ui->AsLink().Call("PumpRequests");
        }

        // Poll Requests
        FOREACH(&Requests, Value, request) {
            request->AsLink().Call("Poll");
        }

        // Run Threads
        FOREACH(&Threads, Value, thread) {
            thread->AsLink().Call("Poll");
        }

        // Output to user
        FOREACH(&UIs, Value, ui) {
            ui->AsLink().Call("Output");
        }

        timer.wait_out();
    }

    goto MAINLOOP;
}
