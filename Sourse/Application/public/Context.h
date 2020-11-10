#pragma once

#include "FrameBuff.h"
#include "LinkedList.h"
#include "Object.h"
#include "Operator.h"
#include "Stack.h"
#include "UI.h"
#include "Vec2.h"
#include "RayCast.h"

#define DKS = KeyState::EVENT_NONE;

enum class KeyState {
  EVENT_NONE = 0,
  HOLD,
  PRESSED,
  RELEASED,
  //DOUBLE_CLICK,
};

struct AppEvent {
  KeyState LMB DKS;
  KeyState RMB DKS;
  KeyState MMB DKS;

  KeyState A DKS;
  KeyState B DKS;
  KeyState C DKS;
  KeyState D DKS;
  KeyState ENTER DKS;
  KeyState SPACE DKS;
  KeyState KEY_1 DKS;
  KeyState KEY_2 DKS;
  KeyState KEY_3 DKS;
  // ...

  vec2<SCR_UINT> Cursor;
  vec2<SCR_UINT> PrevCursor;

  bool IsEvent = true;
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