#pragma once

#include "FrameBuff.h"
#include "LinkedList.h"
#include "Object.h"
#include "Operator.h"
#include "Stack.h"
#include "UI.h"
#include "Vec2.h"
#include "RayCast.h"

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

  vec2<SCR_UINT> Cursor;
  vec2<SCR_UINT> PrevCursor;

  AppEvent();
  ~AppEvent();
  bool IsEvent();
  
};


class AppWindow {
 public:
  AppWindow(FBuff* Sysoutput);
  ~AppWindow();

  // Screen buffer that will see the User
  FBuff* fbuff;

  // All execute commands from UI
  Stack<Operator> op_exec_queue;

  // Screen areas that represents any editor type
  List<ScrArea> scrAreas;
  List<vec2<SCR_UINT>> scrVerts;
  List<Edge<SCR_UINT>> scrEdges;

 public:
  void Draw(class Context* C);

};

class Context {
 public:
  Context(void (*SysOutput)(class SystemHandler* SysH),
          void (*Inputs)(class SystemHandler* SysH, AppEvent* Event),
          FBuff* SysBuff, class SystemHandler* SysH);
  ~Context();

  class SystemHandler* SysH = nullptr;
  void (*SysOutput)(class SystemHandler* SysH) = nullptr;
  void (*SysInput)(class SystemHandler* SysH, AppEvent* ThisEvent) = nullptr;

  RayCast::RenderSettings RndrSets;
  AppWindow window;
  AppEvent event;
  List<Editor> editors;
  List<Operator> operators;
  List<Object> collection;
};