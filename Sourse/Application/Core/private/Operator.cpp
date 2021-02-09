#include "Core/Operator.h"

#include "Core/Seance.h"
#include "UI/UInputsMap.h"
#include "UI/UInterface.h"
#include "UI/UInputs.h"
#include "..//RenderEngines/RayCast/RayCast.h"
#include "Platform/SysHandler.h"
#include "Types.h"

#include <stdlib.h>

#define OPDATA_FROM_OP(Type, name) Type* name = (Type*)op->CustomData;

Operator* find_op(List<Operator>* operators, Str* op_idname) {
  Operator* target = nullptr;
  Range bnds = Range(0, op_idname->len());
  FOREACH_NODE(Operator, operators, op_node) {
    if (op_node->Data->idname.match(bnds, *op_idname, bnds)) {
      target = op_node->Data;
      break;
    }
  }
  return target;
}

// -----------  End Seance Operator ----------------------- //

void EndSeance_ecec(Seance* C, Operator* op) {

  DELETE_DBG(Seance, C);

  #ifdef MEM_DEBUG
  if (LogHeap()) {
    exit(0);
  }
  TreadSleep(10000);
  #endif

  exit(0);
}

void EndSeance_invoke(Seance* C, Operator* op) {
  EndSeance_ecec(C, op);
}

// Checks if operator can be inveked
bool EndSeance_poll(Seance* C, Operator* op) {
  return true;
}

void EndSeance_create(Seance* C, Operator* op) {
  op->idname = "End Seance";
  op->Poll = EndSeance_poll;
  op->Invoke = EndSeance_invoke;
  op->Execute = EndSeance_ecec;

  op->state = OpState::NONE;
}

// -----------  End Seance Operator ----------------------- //

void LogHeap_ecec(Seance* C, Operator* op) {
  #ifdef MEM_DEBUG 
  LogHeap();
  #endif
  op->state = OpState::FINISHED;
}

void LogHeap_invoke(Seance* C, Operator* op) {
  LogHeap_ecec(C, op);
}

// Checks if operator can be inveked
bool LogHeap_poll(Seance* C, Operator* op) {
  return true;
}

void LogHeap_create(Seance* C, Operator* op) {
  op->idname = "Log Heap";
  op->Poll = LogHeap_poll;
  op->Invoke = LogHeap_invoke;
  op->Execute = LogHeap_ecec;

  op->state = OpState::NONE;
}


// -----------  Console Toggle Operator ----------------------- //

void ToggleConcole_ecec(Seance* C, Operator* op) {
  C->ui.sysh->ConsoleToggle();
  op->state = OpState::FINISHED;
}

void ToggleConcole_invoke(Seance* C, Operator* op) {
  ToggleConcole_ecec(C, op);
}

// Checks if operator can be inveked
bool ToggleConcole_poll(Seance* C, Operator* op) {
  return true;
}

void ToggleConcole_create(Seance* C, Operator* op) {
  op->idname = "Toggle Console";
  op->Poll = ToggleConcole_poll;
  op->Invoke = ToggleConcole_invoke;
  op->Execute = ToggleConcole_ecec;

  op->state = OpState::NONE;
}

// -----------  Window Resize Operator ----------------------- //

struct WinResizeData {

  bool top = false;
  bool right = false;
  bool bottom = false;
  bool left = false;

  UIItem* target = nullptr;
};


void WindowResize_ecec(Seance* C, Operator* op) {
  op->state = OpState::FINISHED;
}

void WindowResize_invoke(Seance* C, Operator* op) {
  WinResizeData* data = (WinResizeData*)op->CustomData;
  data->target = C->ui.UIroot->active_lower();

  vec2<float> crsr;
  crsr.assign(C->ui.kmap->uinputs->Cursor.x, C->ui.kmap->uinputs->Cursor.y);
  vec2<float> wrldpos;
  data->target->WrldPos(wrldpos);
  crsr -= wrldpos;
  float fracx = data->target->rect.size.x / 3.f;
  float fracy = data->target->rect.size.y / 3.f;

  data->top = crsr.y > fracy * 2.f;
  data->right = crsr.x > fracx * 2.f;
  data->bottom = crsr.y < fracy;
  data->left = crsr.x < fracx;

  op->state = OpState::RUNNING_MODAL;
}

// Checks if operator can be inveked
bool WindowResize_poll(Seance* C, Operator* op) {
  UIItem* target = C->ui.UIroot->active_lower();
  if (target->hrchy.prnt) {
    op->CustomData = NEW_DBG(WinResizeData) WinResizeData();
    return true;
  }
  return false;
}

void WindowResize_modal(Seance* C, Operator* op, OpArg* event) {
  WinResizeData* dt = (WinResizeData*)op->CustomData;

  if (event && event->idname == "FINISH") {
    op->state = OpState::FINISHED;
    return;
  }
  
  int dx = C->ui.kmap->uinputs->Cdelta.x;
  int dy = C->ui.kmap->uinputs->Cdelta.y;

  Rect<float> rect(dt->target->rect);
  rect.size.y += dy * dt->top;
  rect.size.x += dx * dt->right;

  if (dt->bottom) {
    rect.pos.y += dy;
    rect.size.y -= dy;
  }

  if (dt->left) {
    rect.pos.x += dx;
    rect.size.x -= dx;
  }

  dt->target->Resize(rect);
}

void WindowResize_create(Seance* C, Operator* op) {
  op->state = OpState::NONE;
  op->CustomData = NEW_DBG(WinResizeData) WinResizeData();

  op->idname = "Resize Window";
  op->Poll = WindowResize_poll;
  op->Invoke = WindowResize_invoke;
  op->Modal = WindowResize_modal;

  op->modal_events.add(NEW_DBG(OpArg) OpArg("FINISH"));
}


// -----------  Window Drag Operator ----------------------- //

struct Move {
  vec2<float> startpos;
  vec2<float> startcrs;
  UIItem* target = nullptr;
};

void WindowDrag_ecec(Seance* C, Operator* op) {}

void WindowDrag_invoke(Seance* C, Operator* op) {
  Move* dt = (Move*)op->CustomData;
  dt->startcrs.assign(C->ui.kmap->uinputs->Cursor.x, C->ui.kmap->uinputs->Cursor.y);
  dt->startpos.assign(dt->target->rect.pos.x, dt->target->rect.pos.y);
  op->state = OpState::RUNNING_MODAL;
}

// Checks if operator can be inveked
bool WindowDrag_poll(Seance* C, Operator* op) {
  UIItem* target = C->ui.UIroot->active_lower();
  if (target->hrchy.prnt) {
    Move* mvdt = NEW_DBG(Move) Move();
    mvdt->target = target;
    op->CustomData = mvdt;
    return true;
  }
  return false;
}

void WindowDrag_modal(Seance* C, Operator* op, OpArg* event) {
  Move* dt = (Move*)op->CustomData;

  if (event && event->idname == "FINISH") {
    op->state = OpState::FINISHED;
    op->CustomData = nullptr;
    return;
  }

  vec2<float> crs;
  crs.assign(C->ui.kmap->uinputs->Cursor.x, C->ui.kmap->uinputs->Cursor.y);
  vec2<float> delta = crs - dt->startcrs;

  dt->target->prev_rect = dt->target->rect;

  dt->target->rect.pos.x = dt->startpos.x + delta.x;
  dt->target->valid_resize(dt->target->rect, 0);

  dt->target->rect.pos.y = dt->startpos.y + delta.y;
  dt->target->valid_resize(dt->target->rect, 1);

}

void WindowDrag_create(Seance* C, Operator* op) {
  op->idname = "Move Window";
  op->Poll = WindowDrag_poll;
  op->Modal = WindowDrag_modal;
  op->Invoke = WindowDrag_invoke;

  op->state = OpState::NONE;
}

// -----------  Render To Buff Operator ----------------------- //

void RenderToBuff_ecec(Seance* C, Operator* op) {

  RenderSettings* rd = ((Object*)op->Props.Pointers_Obj[0]->obj)->GetRenderComponent();
  RenderToBuff(rd, (FBuff<RGBA_32>*)op->Props.Pointers_Buff[0]->buff);

  op->state = OpState::FINISHED;
}

// Checks if operator can be inveked
bool RenderToBuff_poll(Seance* C, Operator* op) {
  return true;
}

void RenderToBuff_create(Seance* C, Operator* op) {
  op->idname = "Render To Buff";
  op->Poll = RenderToBuff_poll;
  op->Execute = RenderToBuff_ecec;

  op->Props.Pointers_Obj.add(NEW_DBG(PropertyObjectPtr) PropertyObjectPtr());
  op->Props.Pointers_Buff.add(NEW_DBG(PropertyBuffPtr) PropertyBuffPtr());

  op->state = OpState::NONE;
}

// -----------  Add Plane Atatic Mesh Operator ----------------------- //

void AddPlane_ecec(Seance* C, Operator* op) {

  Object* MeshObj = NEW_DBG(Object) Object();
  C->objects.add(MeshObj);

  StaticMesh* mesh = NEW_DBG(StaticMesh) StaticMesh();
  MeshObj->SetStaticMeshComponent(mesh);

  Trig* trig = NEW_DBG(Trig) Trig();
  trig->V0.assign(0, 0, -1);
  trig->V1.assign(1, 1, -1);
  trig->V2.assign(0, 1, -1);

  mesh->Trigs.add(trig);

  Object* CamObj = NEW_DBG(Object) Object();
  C->objects.add(CamObj);
  CamObj->Pos.z += 2;

  Camera* cam = NEW_DBG(Camera) Camera();
  cam->Lens.setVal(1);

  CamObj->SetCameraComponent(cam);

  Object* RndObj = NEW_DBG(Object) Object();
  C->objects.add(RndObj);

  RenderSettings* rs = NEW_DBG(RenderSettings) RenderSettings();
  RndObj->SetRenderComponent(rs);

  rs->setCamera(CamObj);
  rs->setObjList(&C->objects);

  op->state = OpState::FINISHED;
}

// Checks if operator can be inveked
bool AddPlane_poll(Seance* C, Operator* op) {
  return true;
}

void AddPlane_create(Seance* C, Operator* op) {
  op->state = OpState::NONE;

  op->idname = "Add Plane";
  op->Poll = AddPlane_poll;
  op->Execute = AddPlane_ecec;

  op->modal_events.add(NEW_DBG(OpArg) OpArg("FINISH"));
}

// -----------  Operator ----------------------- //

void AddOperator(Seance* C, void (*Create)(Seance* C, Operator* op)) {
  Operator* op = NEW_DBG(Operator) Operator;
  Create(C, op);
  C->operators.add(op);
}

OpThread::OpThread(Operator* op, OpEvState op_event, OpArg* modalevent)
    : op(op), modalevent(modalevent), op_event(op_event) {

  state = ThreadState::RUNNING;
}

void initOps(Seance* C) {
  AddOperator(C, EndSeance_create);
  AddOperator(C, ToggleConcole_create);
  AddOperator(C, WindowResize_create);
  AddOperator(C, WindowDrag_create);
  AddOperator(C, RenderToBuff_create);
  AddOperator(C, AddPlane_create);
  AddOperator(C, LogHeap_create);
}

Operator::~Operator() {
  if (CustomData) {
    FREE(CustomData);
  }
  modal_events.del();
}
