
#include "App.h"

#include "UI/GUI.h"
#include "UI/TUI.h"

#include "Primitives.h"

#include "Defaults/Operators.h"
#include "Defaults/Widgets.h"

#include "Thread/ThreadManager.h"

#include "Keyboard/Keyboard.h"

Application::Application(Obj* prnt) : Obj (prnt) {
    
    RegisterType(ObjType("Application"));

    ADDOBJ(ObList, UIs, *this, (this)).Assign("UI", true);
    ADDOBJ(ObList, Requests, *this, (this)).Assign("Operator", true);
    ADDOBJ(ObList, Threads, *this, (this)).Assign("Operator", true);
    ADDOBJ(ObList, OpHolders, *this, (this)).Assign("OpHolder", false);

    Obj& data = ADDOBJ(Obj, Data, *this, (this));
    ADDOBJ(Int, fps, data, (&data)).Assign(60, 10, 120);
}

KeyInput* AddKeyInput(TUI* tui, const Str& keyname, const Str& shifted, int key_code, bool text_input) {
  KeyInput* input = new KeyInput(&GETOBJ(ObList, tui, Inputs));

  GETOBJ(Int, input, Key Code).Set(key_code);
  GETOBJ(Bool, input, Is Text Input).Set(text_input);
  GETOBJ(String, input, Char Val).Assign(keyname);
  GETOBJ(String, input, Shifted Char Val).Assign(shifted);

  GETOBJ(ObList, tui, Inputs).AddObj(input);
  return input;
}

void Application::Compose() {
    
    // Adding Operators
    ObList& OpHolders = GETOBJ(ObList, this, OpHolders);
    Operator* op = new QuitProgram(nullptr);
    OpHolder* opholder = new OpHolder(&OpHolders, op);
    OpHolders.AddObj(opholder);

    // Adding TUI
    TUI* tui = new TUI(&GETOBJ(ObList, this, UIs));
    GETOBJ(ObList, this, UIs).AddObj(tui);

    // Adding Inputs
    {
      AddKeyInput(tui, "a", "A", 'A', 1);
      AddKeyInput(tui, "b", "B", 'B', 1);
      AddKeyInput(tui, "c", "C", 'C', 1);
      AddKeyInput(tui, "d", "D", 'D', 1);
      // ...

      AddKeyInput(tui, "1", "!", '1', 1);
      AddKeyInput(tui, "2", "@", '2', 1);
      AddKeyInput(tui, "3", "#", '3', 1);
      AddKeyInput(tui, "4", "$", '4', 1);
      AddKeyInput(tui, "5", "%", '5', 1);
      AddKeyInput(tui, "6", "^", '6', 1);
      AddKeyInput(tui, "7", "&", '7', 1);
      AddKeyInput(tui, "8", "*", '8', 1);
      AddKeyInput(tui, "9", "(", '9', 1);
      AddKeyInput(tui, "0", ")", '0', 1);

      AddKeyInput(tui, ".", ">", KEY_PERIOD, 1);

      AddKeyInput(tui, " ", " ", KEY_SPACE, 1);

      KeyInput* key_shift = AddKeyInput(tui, "Shift", "", KEY_SHIFT, 0);
      GETOBJ(Link, tui, Shift Key).SetLink(key_shift);
    }
    KeyInput* key_LMB = AddKeyInput(tui, "LMB", "", KEY_LBUTTON, 0);
    KeyInput* key_RMB = AddKeyInput(tui, "RMB", "", KEY_RBUTTON, 0);
    KeyInput* key_del = AddKeyInput(tui, "DELETE", "", KEY_DELETE, 0);

    // Adding Shortcuts
    ShortCut* shcut = new ShortCut(&GETOBJ(ObList, tui, Shortcuts));
    GETOBJ(Link, shcut, Target Op).SetLink(opholder);

    ObList& cond_list = GETOBJ(ObList, &GETOBJ(CompareExpr, shcut, Invoke), Conditions);

    ObjTuple* cond = new ObjTuple(&cond_list);
    Int* trigger_val = new Int(nullptr);
    trigger_val->Assign(3, -1, 5);
    cond->GetHead().SetLink(trigger_val);
    cond->GetTail().SetLink(&GETOBJ(Int, key_del, State));
    cond_list.AddObj(cond);
    
    GETOBJ(ObList, tui, Shortcuts).AddObj(shcut);


    // Adding GUI
    GUI* gui = new GUI(&GETOBJ(ObList, this, UIs), tui);
    GETOBJ(ObList, this, UIs).AddObj(gui);
    
    Obj& Trigers = GETOBJ(Obj, gui, Trigers);
    ObList& act_cond_list = GETOBJ(ObList, &GETOBJ(CompareExpr, &Trigers, Activate), Conditions);
    ObList& discard_cond_list = GETOBJ(ObList, &GETOBJ(CompareExpr, &Trigers, Discard), Conditions);
    ObList& comfirm_cond_list = GETOBJ(ObList, &GETOBJ(CompareExpr, &Trigers, Comfirm), Conditions);

    ObjTuple* close_cond = new ObjTuple(&discard_cond_list);
    Int* close_trigger_val = new Int(nullptr);
    close_trigger_val->Assign((int)InputState::RELEASED, -1, 5);
    close_cond->GetHead().SetLink(close_trigger_val);
    close_cond->GetTail().SetLink(&GETOBJ(Int, key_RMB, State));
    discard_cond_list.AddObj(close_cond);

    ObjTuple* comfirm_cond = new ObjTuple(&comfirm_cond_list);
    Int* comfirm_trigger_val = new Int(nullptr);
    comfirm_trigger_val->Assign((int)InputState::RELEASED, -1, 5);
    comfirm_cond->GetHead().SetLink(comfirm_trigger_val);
    comfirm_cond->GetTail().SetLink(&GETOBJ(Int, key_LMB, State));
    comfirm_cond_list.AddObj(comfirm_cond);

    ObjTuple* activate_cond = new ObjTuple(&comfirm_cond_list);
    Int* act_trigger_val = new Int(nullptr);
    act_trigger_val->Assign((int)InputState::PRESSED, -1, 5);
    activate_cond->GetHead().SetLink(act_trigger_val);
    activate_cond->GetTail().SetLink(&GETOBJ(Int, key_LMB, State));
    act_cond_list.AddObj(activate_cond);

    ContextMenu* ctx_menu = new ContextMenu(gui, Rect<float>(50, 50, 300, 500));
    GETOBJ(Link, ctx_menu, Target).SetLink(this);
    GETOBJ(ObList, gui, Windows).AddObj(ctx_menu);
}

void Application::Run() {

    ObList& UIs = GETOBJ(ObList, this, UIs);
    ObList& Threads = GETOBJ(ObList, this, Threads);
    ObList& Requests = GETOBJ(ObList, this, Requests);

    MAINLOOP : {

         Timer timer = Timer((uint8)(1000.f / GETOBJ(Int, &GetChld("Data"), fps).GetVal()));

        // Pump Requests From UIs
        for (auto ui : UIs.GetList()) {
            ((UI*)ui.Data())->PumpRequests(&Requests);
        }

        // Poll Requests
        for (auto request : Requests.GetList()) {
            Operator* op = (Operator*)request.Data();
            if (op->Poll()) {
                Requests.GetList().Detach(request.node());
                Threads.GetList().PushBack(request.node());

                op->Invoke();
            }
        }

        // Run Threads
        for (auto thread : Threads.GetList()) {
            Operator* op = (Operator*)thread.Data();

            op->Modal();
            
            if (op->Finished()) {
                Threads.GetList().Detach(thread.node());
            }
        }   

        // Output to user
        for (auto ui : UIs.GetList()) {
            ((UI*)ui.Data())->OutPut(prnt);
        }

        timer.wait_out();

    } goto MAINLOOP;
}