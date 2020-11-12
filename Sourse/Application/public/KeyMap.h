
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

struct UserInputs {
  USRINPUT_DECL(A);
  USRINPUT_DECL(B);
  USRINPUT_DECL(C);
  // ...
  USRINPUT_DECL(END_OF_INPUTS);

  vec2<SCR_UINT> Cursor;
  vec2<SCR_UINT> PrevCursor;
};

// User defined key map
struct KeyCondition {
  std::string input_idname;
  InputState trigger_state;
};

struct Shortcut {
  Stack<KeyCondition> conditions;
  struct ModalEvent modal_event;
};

struct OpBindings {
  std::string op_name;
  Shortcut invoke;
  Stack<Shortcut> modal_keymap;
};

struct UserKeyMap {
  List<OpBindings> map;
};



// compiled map for practical use
struct CKeyCondition {
  InputState* input_ptr;
  InputState trigger_state;

  bool Compile(UserInputs* usr_inputs, KeyCondition* usr_cond);

  bool IsTrue();
};

struct CShortcut {
  Stack<CKeyCondition> conditions;
  struct ModalEvent modal_event;

  bool Compile(UserInputs* usr_inputs, Shortcut* shortcut);

  bool IsShortcut();
};

struct COpBindings {
  CShortcut invoke;
  Stack<CShortcut> modal_keymap;
  struct Operator* op_ptr;
  struct OpThread* thread_ptr;

  bool Compile(std::string* operator_idname,
               List<struct Operator>* operators,
               UserInputs* usr_inputs,
               OpBindings* user_op_bindings);

  ModalEvent* IsModalEvent();
  bool IsInvoked();
};

// one per window
struct CompiledKeyMap {
  Stack<COpBindings> op_bindings;

  void Compile(List<struct Operator>* operators,
               UserKeyMap* user_key_bidings,
               UserInputs* usr_inputs);

  void ProcEvents(List<OpThread>* exec_queue);
};