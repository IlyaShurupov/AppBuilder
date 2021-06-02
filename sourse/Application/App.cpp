
#include "App.h"

#include "UI/uis/GUI.h"
#include "UI/uis/TUI.h"

#include "Primitives.h"

#include "Defaults/Operators.h"
#include "Defaults/Guiis.h"

#include "Thread/ThreadManager.h"

#include "Device/Device.h"

Application::Application(Obj* prnt) : Obj (prnt) {
    
    RegisterType(ObjType("Application"));

    ADDOBJ(ObList, UIs, *this, (this)).Assign("UI", true);
    ADDOBJ(ObList, Requests, *this, (this)).Assign("Operator", true);
    ADDOBJ(ObList, Threads, *this, (this)).Assign("Operator", true);
    ADDOBJ(ObList, OpHolders, *this, (this)).Assign("OpHolder", false);

    Obj& data = ADDOBJ(Obj, Data, *this, (this));
    ADDOBJ(Int, fps, data, (&data)).Assign(60, 10, 120);
}

void Application::Compose() {
    Device* dev = new Device();

    
    // Adding Operators
    ObList& OpHolders = GETOBJ(ObList, this, OpHolders);
    Operator* op = new QuitProgramm(nullptr);
    OpHolder* opholder = new OpHolder(nullptr, op);
    OpHolders.AddObj(opholder);

    // Adding TUI
    TUI* tui = new TUI(this, dev);
    GETOBJ(ObList, this, UIs).AddObj(tui);

    // Adding Inputs
    KeyInput* input = new KeyInput(nullptr);
    GETOBJ(String, input, KeyName).Assign("A");
    GETOBJ(Int, input, ASCII Code).Set('A');
    GETOBJ(ObList, tui, Inputs).AddObj(input);

    KeyInput* key_RMB = new KeyInput(nullptr);
    GETOBJ(String, key_RMB, KeyName).Assign("RMB");
    GETOBJ(Int, key_RMB, ASCII Code).Set(KEY_RBUTTON);
    GETOBJ(ObList, tui, Inputs).AddObj(key_RMB);

    // Adding Shortcuts
    ShortCut* shcut = new ShortCut(nullptr);
    GETOBJ(Link, shcut, Target Op).SetLink(opholder);

    ObList& cond_list = GETOBJ(ObList, &GETOBJ(CompareExpr, shcut, Invoke), Conditions);

    ObjTuple* cond = new ObjTuple(&cond_list);
    Int* trigger_val = new Int(nullptr);
    trigger_val->Assign(3, -1, 5);
    cond->GetHead().SetLink(trigger_val);
    cond->GetTail().SetLink(&GETOBJ(Int, input, State));
    cond_list.AddObj(cond);
    
    GETOBJ(ObList, tui, Shortcuts).AddObj(shcut);


    // Adding GUI
    GUI* gui = new GUI(this, dev);
    GETOBJ(ObList, this, UIs).AddObj(gui);
    
    Obj& Trigers = GETOBJ(Obj, gui, Trigers);
    ObList& act_cond_list = GETOBJ(ObList, &GETOBJ(CompareExpr, &Trigers, Activate), Conditions);
    ObList& close_cond_list = GETOBJ(ObList, &GETOBJ(CompareExpr, &Trigers, Close), Conditions);

    ObjTuple* close_cond = new ObjTuple(&close_cond_list);
    Int* close_trigger_val = new Int(nullptr);
    close_trigger_val->Assign(3, -1, 5);
    close_cond->GetHead().SetLink(close_trigger_val);
    close_cond->GetTail().SetLink(&GETOBJ(Int, key_RMB, State));
    close_cond_list.AddObj(close_cond);

    ObjTuple* act_cond = new ObjTuple(&act_cond_list);
    Int* act_trigger_val = new Int(nullptr);
    act_trigger_val->Assign(3, -1, 5);
    act_cond->GetHead().SetLink(act_trigger_val);
    act_cond->GetTail().SetLink(&GETOBJ(Int, key_RMB, State));
    act_cond_list.AddObj(act_cond);


    ContextMenu* ctx_menu = new ContextMenu(nullptr, Rect<float>(50, 50, 300, 500));
    GETOBJ(Link, ctx_menu, Target).SetLink(this);
    GETOBJ(ObList, gui, Windows).AddObj(ctx_menu);
}

void Application::Run() {

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