#pragma once

#include "Event.h"
#include "FrameBuff.h"
#include "LinkedList.h"
#include "Object.h"
#include "Operator.h"
#include "Vec2.h"

enum KeyState {
  EVENT_NONE = 0,
  PRESSED,
  RELEASED,
  HOLD,
  DOUBLE,
};

struct EventState {
  KeyState A;
  KeyState B;
  KeyState C;
  KeyState D;
  KeyState ENTER;
  KeyState SPACE;
  KeyState KEY_1;
  KeyState KEY_2;
  KeyState KEY_3;
  // ...
  short num = 9;
  vec2<short> Cursor;
  vec2<short> dCursor;
};

class Window {
 public:
  vec2<short> WorldPos;
  vec2<short> Size;

  EventState EventState;
  FBuff* Fbuff;

  Window();
  ~Window();

  void UpdEventState();
  bool IsEvent();
  void Draw(class Context* C);
};

class Context {
 public:
  Context();
  ~Context();

  List<Window> Windows;
  List<Object> Collection;
  List<Operator> Operators;

  Window* ActiveWin = nullptr;

  Window* getActiveWindow();
};