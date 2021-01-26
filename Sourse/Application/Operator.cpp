#include "public/Operator.h"
#include "public/Print.h"
#include "public/Seance.h"
#include "../RenderEngines/RayCast/public/RayCast.h"

#define OPDATA_FROM_OP(Type, name) Type* name = (Type*)op->CustomData;
#define RET_FINISHED(op)         \
  op->State = OpState::FINISHED; \
  return;

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
  op->CustomData = DBG_NEW opnameData();

  op->idname = "opname";
  op->Poll = opname_poll;
  op->Invoke = opname_invoke;
  op->Modal = opname_modal;

  op->modal_events.add(DBG_NEW ModalEvent("FINISH"));
}
*/

// -----------  End Seance Operator ----------------------- //

void EndSeance_ecec(Seance* C, Operator* op) {
  // save &
  delete C;
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


void WindowResize_ecec(Seance* C, Operator* op) { op->state = OpState::FINISHED; }

void WindowResize_invoke(Seance* C, Operator* op) {
  WinResizeData* data = (WinResizeData*)op->CustomData;

  vec2<SCR_UINT>* crsr = &data->win->user_inputs.Cursor;
  Rect<SCR_UINT> rect;
  data->win->getRect(rect);

  float fracx = rect.size.x / 3.f;
  float fracy = rect.size.y / 3.f;

  data->top = crsr->y > fracy * 2.f;
  data->right = crsr->x > fracx * 2.f;
  data->bottom = crsr->y < fracy;
  data->left = crsr->x < fracx;

  op->state = OpState::RUNNING_MODAL;
}

// Checks if operator can be inveked
bool WindowResize_poll(Seance* C, Operator* op) {
  WinResizeData* data = (WinResizeData*)op->CustomData;
  return data->win = C->project.C_actWin();
}

void WindowResize_modal(Seance* C, Operator* op, ModalEvent* event) {
  WinResizeData* data = (WinResizeData*)op->CustomData;

  if (event && event->idname == "FINISH") {
    op->state = OpState::FINISHED;
    return;
  }

  int dx = data->win->user_inputs.Cdelta.x;
  int dy = data->win->user_inputs.Cdelta.y;

  Rect<SCR_UINT> rect;
  data->win->getRect(rect);

  //rect.size.y += 2;
  //rect.pos.y += -1;
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
  op->CustomData = DBG_NEW WinResizeData();

  op->idname = "Resize Window";
  op->Poll = WindowResize_poll;
  op->Invoke = WindowResize_invoke;
  op->Modal = WindowResize_modal;

  op->modal_events.add(DBG_NEW ModalEvent("FINISH"));
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

void WindowDrag_modal(Seance* C, Operator* op, ModalEvent* event) {
  Window* data = (Window*)op->CustomData;

  if (event && event->idname == "FINISH") {
    op->state = OpState::FINISHED;
    op->CustomData = nullptr;
    return;
  }

  Rect<SCR_UINT> rect;
  data->getRect(rect);
  rect.move(data->user_inputs.Cdelta.x, data->user_inputs.Cdelta.y);
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

  op->Props.Pointers_Obj.add(DBG_NEW PropertyObjectPtr());
  op->Props.Pointers_Buff.add(DBG_NEW PropertyBuffPtr());

  op->state = OpState::NONE;
}

// -----------  Add Plane Atatic Mesh Operator ----------------------- //

void AddPlane_ecec(Seance* C, Operator* op) {

  Object* MeshObj = DBG_NEW Object();
  StaticMesh* mesh = DBG_NEW StaticMesh();

  Trig* trig = DBG_NEW Trig();
  trig->V0.assign(0, 0, -1);
  trig->V1.assign(1, 1, -1);
  trig->V2.assign(0, 1, -1);

  mesh->Trigs.add(trig);
  MeshObj->SetStaticMeshComponent(mesh);

  Camera* cam = DBG_NEW Camera();
  cam->Height.setVal(200);
  cam->Width.setVal(200);
  cam->Lens.setVal(1);

  Object* CamObj = DBG_NEW Object();
  CamObj->SetCameraComponent(cam);


  RenderSettings* rs = DBG_NEW RenderSettings();
  rs->setCamera(CamObj);
  rs->setObjList(&C->project.collection);
 
  Object* RndObj = DBG_NEW Object();
  RndObj->SetRenderComponent(rs);

  C->project.collection.add(CamObj);
  C->project.collection.add(MeshObj);
  C->project.collection.add(RndObj);

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

  op->modal_events.add(DBG_NEW ModalEvent("FINISH"));
}

// -----------  Operator ----------------------- //

void AddOperator(Seance* C, void (*Create)(Seance* C, Operator* op)) {
  Operator* op = DBG_NEW Operator;
  Create(C, op);
  C->prefferences.operators.add(op);
}

OpThread::OpThread(Operator* op, OpEventState op_event, ModalEvent* modal_event)
    : op(op), modal_event(modal_event), op_event(op_event) {

  state = ThreadState::RUNNING;
}

void initOps(Seance* C) {
  AddOperator(C, EndSeance_create);
  AddOperator(C, ToggleConcole_create);
  AddOperator(C, WindowResize_create);
  AddOperator(C, WindowDrag_create);
  AddOperator(C, RenderToBuff_create);
  AddOperator(C, AddPlane_create);
}

Operator::~Operator() {
  if (CustomData) {
    delete CustomData;
  }
  modal_events.del();
}
