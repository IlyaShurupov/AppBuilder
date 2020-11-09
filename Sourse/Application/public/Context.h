#pragma once

#include "FrameBuff.h"
#include "LinkedList.h"
#include "Object.h"
#include "Operator.h"
#include "Stack.h"
#include "UI.h"
#include "Vec2.h"

enum class KeyState {
  EVENT_NONE = 0,
  PRESSED,
  RELEASED,
  HOLD,
  //DOUBLE_CLICK,
};

class AppEvent {
 public:
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
  short KeyNum = 9;

  vec2<short> Cursor;
  vec2<short> PrevCursor;

  AppEvent(void (*Update)(AppEvent* ThisEvent));
  ~AppEvent();
  bool IsEvent();
  void (*Update)(AppEvent* ThisEvent);
};


class AppWindow {
 public:
  AppWindow(void (*SysOutput)(), FBuff* Sysoutput);
  ~AppWindow();

  // Screen buffer that will see the User
  FBuff* fbuff;

  // All execute commands from UI
  Stack<Operator> op_exec_queue;

  // Screen areas that represents any editor type
  List<ScrArea> scrAreas;
  List<vec2<short>> scrVerts;
  List<Edge<short>> scrEdges;

 public:
  void Draw(class Context* C);
  void (*SysOutput)();
};

class Context {
 public:
  Context(void (*SysOutput)(), void (*Inputs)(AppEvent* Event),
          FBuff* Sysoutput);
  ~Context();

  AppWindow window;
  AppEvent event;
  List<Editor> editors;
  List<Operator> operators;
  List<Object> collection;
};