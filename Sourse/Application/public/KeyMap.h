
#pragma once

#include "LinkedList.h"
#include "Stack.h"
#include "Vec2.h"
#include "public/Operator.h"

#define USRINPUT_DECL(INPUT_NAME) Input INPUT_NAME = Input(NAME(INPUT_NAME), InputState::NONE)

enum class InputState {
  NONE = 0,
  HOLD,
  PRESSED,
  RELEASED,
};

struct Input {
  Input();
  Input(std::string idname, InputState state = InputState::NONE);
  std::string idname;
  InputState state = InputState::NONE;
};

typedef long long int strloc;

struct Bounds {
  strloc len() { return end - strt;}
  Bounds(){}
  Bounds(strloc strt, strloc end) {
    this->strt = strt;
    this->end = end;
  }
  strloc strt = -1;
  strloc end = -1;
};

struct UserInputs {

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

  Input A = Input("A", InputState::NONE);
  Input D = Input("D", InputState::NONE);
  //USRINPUT_DECL(A);
  USRINPUT_DECL(S);
  //USRINPUT_DECL(D);
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
  USRINPUT_DECL(END_OF_INPUTS);


  vec2<SCR_UINT> Cursor;
  vec2<SCR_UINT> PrevCursor;
};


// compiled map for practical use
struct CKeyCondition {
  InputState* input_ptr;
  InputState trigger_state;

  bool Compile(UserInputs* usins, std::string* str, Bounds& bnds);

  bool IsTrue();
};

struct CShortcut {
  Stack<CKeyCondition> conditions;
  struct ModalEvent modal_event;

  bool Compile(UserInputs* usins, std::string* str, Bounds& bnds);

  bool IsShortcut();
};

struct COpBindings {
  CShortcut invoke;
  Stack<CShortcut> modal_keymap;
  struct Operator* op_ptr;
  struct OpThread* thread_ptr;

  bool Compile(List<Operator>* ops, UserInputs* usins, std::string* str, Bounds bnds);

  ModalEvent* IsModalEvent();
  bool IsInvoked();
};

// one per window
struct CompiledKeyMap {
  
  Stack<COpBindings> op_bindings;

  void Compile(List<Operator>* ops, UserInputs* usins, std::string* filipath);

  void ProcEvents(List<OpThread>* exec_queue);
};

std::string getExecutablePath();