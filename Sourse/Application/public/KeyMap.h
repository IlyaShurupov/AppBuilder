
#pragma once

#include "LinkedList.h"
#include "Stack.h"
#include "Vec2.h"
#include "public/Operator.h"

#define MAX_KEY_NAME_LENGTH 5

enum class InputState {
  NONE = 0,
  HOLD,
  PRESSED,
  RELEASED,
};

struct UserInputs {
  std::map<char[MAX_KEY_NAME_LENGTH], InputState> map;

  // ...

  vec2<SCR_UINT> Cursor;
  vec2<SCR_UINT> PrevCursor;
};

struct KeyCondition {
  char input_idkey[MAX_KEY_NAME_LENGTH];
  InputState trigger_state;

  bool IsMet(UserInputs* inputs);
};

struct Shortcut {
  Stack<KeyCondition> conditions;
  struct ModalEvent modal_event;

  bool IsShortcut(UserInputs* inputs);
};

struct OperatorBindings {
  Shortcut invoke;
  Stack<Shortcut> modal_keymap;
  struct Operator* op_ptr;

  OperatorBindings(char* operator_idname, List<struct Operator>* operators);

  ModalEvent* IsModalEvent(UserInputs* inputs);
  bool IsInvoked(UserInputs* inputs);
};

struct KeyMap {
  std::map<char*, OperatorBindings*> map;

  void add(struct Operator* op, OperatorBindings* key_bidings);
};
