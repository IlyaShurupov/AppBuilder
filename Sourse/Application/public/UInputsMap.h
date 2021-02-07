
#pragma once

#include "LinkedList.h"
#include "Stack.h"
#include "UInterface.h"
#include "Vec2.h"
#include "public/Operator.h"
#include "public/FileReader.h"

#define USRINPUT_DECL(name) Input name = Input(NAME(name))

enum class InputState {
  NONE = 0,
  HOLD,
  PRESSED,
  RELEASED,
};

struct Input {
  Input(const char* name);

  struct Str idname;
  InputState state = InputState::NONE;
};

struct UInputs {

  bool IsEvent = false;

  USRINPUT_DECL(SYS_DESTROY_COMMAND);

  USRINPUT_DECL(WIN_KEY);
  USRINPUT_DECL(K0);
  USRINPUT_DECL(K1);
  USRINPUT_DECL(K2);
  USRINPUT_DECL(K3);
  USRINPUT_DECL(K4);
  USRINPUT_DECL(K5);
  USRINPUT_DECL(K6);
  USRINPUT_DECL(K7);
  USRINPUT_DECL(K8);
  USRINPUT_DECL(K9);

  USRINPUT_DECL(Q);
  USRINPUT_DECL(W);
  USRINPUT_DECL(E);
  USRINPUT_DECL(R);
  USRINPUT_DECL(T);
  USRINPUT_DECL(Y);
  USRINPUT_DECL(U);
  USRINPUT_DECL(I);
  USRINPUT_DECL(O);
  USRINPUT_DECL(P);

  USRINPUT_DECL(A);
  USRINPUT_DECL(S);
  USRINPUT_DECL(D);
  USRINPUT_DECL(F);
  USRINPUT_DECL(G);
  USRINPUT_DECL(H);
  USRINPUT_DECL(J);
  USRINPUT_DECL(K);
  USRINPUT_DECL(L);

  USRINPUT_DECL(Z);
  USRINPUT_DECL(X);
  USRINPUT_DECL(C);
  USRINPUT_DECL(V);
  USRINPUT_DECL(B);
  USRINPUT_DECL(N);
  USRINPUT_DECL(M);

  USRINPUT_DECL(SPACE);
  USRINPUT_DECL(ENTER);
  USRINPUT_DECL(DEL);
  USRINPUT_DECL(EREASE);
  USRINPUT_DECL(ESCAPE);
  USRINPUT_DECL(TAB);

  USRINPUT_DECL(SHIFT_L);
  USRINPUT_DECL(SHIFT_R);
  USRINPUT_DECL(CTR_L);
  USRINPUT_DECL(CTR_R);
  USRINPUT_DECL(ALT_L);
  USRINPUT_DECL(ALT_R);

  USRINPUT_DECL(ARROW_UP);
  USRINPUT_DECL(ARROW_DOWN);
  USRINPUT_DECL(ARROW_LEFT);
  USRINPUT_DECL(ARROW_RIGHT);

  USRINPUT_DECL(RMB);
  USRINPUT_DECL(LMB);
  USRINPUT_DECL(MMB);

  // ...
  USRINPUT_DECL(_UIEND_);

  vec2<SCR_UINT> Cdelta;
  vec2<SCR_UINT> Cursor;
  vec2<SCR_UINT> PrevCursor;
};

// ----------------------------------------------------------------------------------- //

template <class type>
struct Cond {
  type* target = nullptr;
  type trigger_state;
  bool met() { return *target == trigger_state; }
};

template <class type>
struct Conditions {

  bool any = false;
  List<Cond<type>> conds;

  bool satisfies() {
    FOREACH(&conds, Cond<type>, node) {
      if (node->Data->met()) {
        IF(any, return true);
        continue;
      }
      return false;
    }
    return true;
  }

  type* (*find_target)(Str* string, void* sourse) = nullptr;
  type (*to_state)(Str* string) = nullptr;

  void Compile(DataBlock* db, void* sourse) {

    DataBlock* condlistdb = nullptr;
    any = db->find("Logic")->string == "ANY";
    List<DataBlock>* conds_list = &db->find("List")->list;
    FOREACH(conds_list, DataBlock, node) {
      Cond<type>* cond = NEW_DBG(Cond<type>) Cond<type>();
      cond->target = find_target(&node->Data->find("Name")->string, sourse);
      cond->trigger_state = to_state(&node->Data->find("State")->string);
      conds.add(cond);
    }
  }

  ~Conditions() { conds.del(); }
};

struct Trigger {

  bool runtime = false;
  struct OpArg arg;

  Conditions<enum class UIIstate> scope_conds;
  Conditions<enum class InputState> key_conds;

  bool active();

  void Compile(DataBlock* db, UInputs* uinputs, UIItem* root);
};

struct OPInterface {

  struct Operator* target = nullptr;
  OpThread* op_thread = nullptr;
  List<Trigger> triggers;

  void proc(List<OpThread>* queue);

  void Compile(DataBlock* db, List<Operator>* ops, UInputs* uinputs, UIItem* root);
  ~OPInterface();
};

struct KeyMap {

  List<OPInterface> opiterfaces;  

  void evaluate(List<OpThread>* exec_queue);

  void Compile(DataBlock* db, List<Operator>* ops, UInputs* uinputs, UIItem* root);
};
