
#pragma once

#include "LinkedList.h"
#include "Stack.h"
#include "Vec2.h"

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

struct InputCondition {
  char input_idkey[MAX_KEY_NAME_LENGTH];
  InputState trigger_state;

  bool IsMet(UserInputs* inputs);
};

struct OpEvent {
  char* op_event_idname;
  Stack<InputCondition> conditions;

  bool IsMet(UserInputs* inputs);
};

struct OperatorBinfings {
  Stack<OpEvent> op_events;
  class Operator* op_ptr;

  OperatorBinfings(char* operator_idname, List<class Operator>* operators);

  char* IsOPEvent(UserInputs* inputs);
};

struct KeyMap {
  std::map<char*, OperatorBinfings*> map;

  void add(class Operator* op, OperatorBinfings* key_bidings);
};
