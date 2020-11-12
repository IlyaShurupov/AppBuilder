
#pragma once

#include <string>
#include "LinkedList.h"
#include "Stack.h"
#include "Vec2.h"
#include "public/Operator.h"
#define MAX_INPUT_IDNAME_LENGTH 5

enum class InputState {
  NONE = 0,
  HOLD,
  PRESSED,
  RELEASED,
};

struct Input {
  char idname[MAX_INPUT_IDNAME_LENGTH];
  InputState state;
};

struct UserInputs {
  Input A;
  Input B;
  Input C;
  int length = 3;
  // ...
  vec2<SCR_UINT> Cursor;
  vec2<SCR_UINT> PrevCursor;
};

// User defined key map
struct KeyCondition {
  char input_idname[MAX_INPUT_IDNAME_LENGTH];
  InputState trigger_state;
};

struct Shortcut {
  Stack<KeyCondition> conditions;
  struct ModalEvent modal_event;
};

struct OpBindings {
  char op_name[MAX_OPERATOR_NAME_LENGTH];
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

  bool Compile(char* operator_idname,
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

  void ProcEvents(List<ExecComand>* exec_queue);
};