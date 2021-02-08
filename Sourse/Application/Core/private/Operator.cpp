#include "Core/Operator.h"

#include "Core/Seance.h"
#include "UI/Window.h"
#include "UI/UInputsMap.h"
#include "UI/UInputs.h"
#include "..//RenderEngines/RayCast/RayCast.h"

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
  if (C->project.windows.len())
    C->project.windows[0]->ToggleConsole();
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
  Window* win = nullptr;
};


void WindowResize_ecec(Seance* C, Operator* op) {
  op->state = OpState::FINISHED;
}

void WindowResize_invoke(Seance* C, Operator* op) {
  WinResizeData* data = (WinResizeData*)op->CustomData;

  vec2<SCR_UINT> crsr(data->win->user_inputs->Cursor);
  Rect<SCR_UINT> rect;
  data->win->getRect(rect);
  crsr -= rect.pos;
  float fracx = rect.size.x / 3.f;
  float fracy = rect.size.y / 3.f;

  data->top = crsr.y > fracy * 2.f;
  data->right = crsr.x > fracx * 2.f;
  data->bottom = crsr.y < fracy;
  data->left = crsr.x < fracx;

  op->state = OpState::RUNNING_MODAL;
}

// Checks if operator can be inveked
bool WindowResize_poll(Seance* C, Operator* op) {
  WinResizeData* data = (WinResizeData*)op->CustomData;
  return data->win = C->project.C_actWin();
}

void WindowResize_modal(Seance* C, Operator* op, OpArg* event) {
  WinResizeData* data = (WinResizeData*)op->CustomData;

  if (event && event->idname == "FINISH") {
    op->state = OpState::FINISHED;
    return;
  }

  int dx = data->win->user_inputs->Cdelta.x;
  int dy = data->win->user_inputs->Cdelta.y;

  Rect<SCR_UINT> rect;
  data->win->getRect(rect);

  // rect.size.y += 2;
  // rect.pos.y += -1;
  rect.size.y += dy * data->top;
  rect.size.x += dx * data->right;

  if (data->bottom) {
    rect.pos.y += dy;
    rect.size.y -= dy;
  }

  if (data->left) {
    rect.pos.x += dx;
    rect.size.x -= dx;
  }
  /*
   */

  data->win->setRect(rect);
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

void WindowDrag_ecec(Seance* C, Operator* op) {}

void WindowDrag_invoke(Seance* C, Operator* op) {
  op->state = OpState::RUNNING_MODAL;
}

// Checks if operator can be inveked
bool WindowDrag_poll(Seance* C, Operator* op) {
  return op->CustomData = C->project.C_actWin();
}

void WindowDrag_modal(Seance* C, Operator* op, OpArg* event) {
  Window* data = (Window*)op->CustomData;

  if (event && event->idname == "FINISH") {
    op->state = OpState::FINISHED;
    op->CustomData = nullptr;
    return;
  }

  Rect<SCR_UINT> rect;
  data->getRect(rect);
  rect.move(data->user_inputs->Cdelta.x, data->user_inputs->Cdelta.y);
  data->setRect(rect);
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
  C->project.collection.add(MeshObj);

  StaticMesh* mesh = NEW_DBG(StaticMesh) StaticMesh();
  MeshObj->SetStaticMeshComponent(mesh);

  Trig* trig = NEW_DBG(Trig) Trig();
  trig->V0.assign(0, 0, -1);
  trig->V1.assign(1, 1, -1);
  trig->V2.assign(0, 1, -1);

  mesh->Trigs.add(trig);

  Object* CamObj = NEW_DBG(Object) Object();
  C->project.collection.add(CamObj);
  CamObj->Pos.z += 2;

  Camera* cam = NEW_DBG(Camera) Camera();
  cam->Lens.setVal(1);

  CamObj->SetCameraComponent(cam);

  Object* RndObj = NEW_DBG(Object) Object();
  C->project.collection.add(RndObj);

  RenderSettings* rs = NEW_DBG(RenderSettings) RenderSettings();
  RndObj->SetRenderComponent(rs);

  rs->setCamera(CamObj);
  rs->setObjList(&C->project.collection);
  /*
  */

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
  C->prefferences.operators.add(op);
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
    //DELETE_DBG() CustomData;
  }
  modal_events.del();
}


/*
// -----------  Operator template ----------------------- //

typedef struct opnameData {
  bool top = false;
  bool right = false;
  bool bottom = false;
  bool left = false;
  Window* win = nullptr;
}opnameData;


void opname_ecec(Seance * C, Operator * op) {
  op->state = OpState::FINISHED;
}

void opname_invoke(Seance * C, Operator * op) {
  opnameData* data = (opnameData*)op->CustomData;

  op->state = OpState::RUNNING_MODAL;
}

// Checks if operator can be inveked
bool opname_poll(Seance * C, Operator * op) {
  opnameData* data = (opnameData*)op->CustomData;
  return data->win = C->project.C_actWin();
}

void opname_modal(Seance * C, Operator * op, ModalEvent * event) {
  opnameData* data = (opnameData*)op->CustomData;

  if (event && event->idname == "FINISH") {
    op->state = OpState::FINISHED;
    return;
  }
}

void opname_create(Seance * C, Operator * op) {
  op->state = OpState::NONE;
  op->CustomData = NEW_DBG() opnameData();

  op->idname = "opname";
  op->Poll = opname_poll;
  op->Invoke = opname_invoke;
  op->Modal = opname_modal;

  op->modal_events.add(NEW_DBG() ModalEvent("FINISH"));
}
*/
