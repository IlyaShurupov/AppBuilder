#pragma once

#include "FrameBuff.h"
#include "LinkedList.h"
#include "Object.h"
#include "Operator.h"
#include "Stack.h"
#include "UI.h"
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

typedef struct ScrEdge {
  vec2<short> v1, v2;
} ScrEdge;

class Screen {
 public:
  Screen();
  ~Screen();

  // position & Size
  RectPtr<short> Rect;

  // Screen buffer that will see the User
  FBuff* Fbuff;

  // All execute commands from UI
  Stack<Operator> OpExecQueue;

  // Screen areas that represents any editor type
  List<ScrArea> ScrAreas;
  List<vec2<short>> ScrVerts;
  List<ScrEdge> ScrEdges;

  // Event state from user
  EventState EventState;

 public:
  void UpdEventState();
  bool IsEvent();

  void Draw(class Context* C);
};

class Context {
 public:
  Context();
  ~Context();

  List<Screen> Screens;
  List<Editor> Editors;
  List<Operator> Operators;
  List<Object> Collection;

  Screen* ActiveWin = nullptr;

  Screen* getActiveScreen();
};