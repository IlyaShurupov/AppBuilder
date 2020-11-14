#include "public/Operator.h"

#include "Print.h"
#include "public/Seance.h"
#define OPDATA_FROM_OP(Type, name) Type* name = (Type*)op->CustomData;
#define RET_FINISHED(op)         \
  op->State = OpState::FINISHED; \
  return;

// -----------  End Seance Operator ----------------------- //

void EndSeance_ecec(Seance* C, Operator* op) {
  FOREACH_NODE(Window, (&C->project.windows), win_node) { win_node->Data->Destroy(); }
  exit(0);
}

void EndSeance_invoke(Seance* C, Operator* op) {
  EndSeance_ecec(C, op);
  op->state = OpState::FINISHED;
}

// Checks if operator can be inveked
bool EndSeance_poll(Seance* C, Operator* op) {
  return true;
}

void EndSeance_create(Seance* C, Operator* op) {
  op->idname = "End Seance";
  op->Poll = EndSeance_poll;
  op->Invoke = EndSeance_invoke;

  op->state = OpState::NONE;
}

// -----------  End Seance Operator end ----------------------- //

// -----------  Console Toggle Operator ----------------------- //

void ToggleConcole_ecec(Seance* C, Operator* op) {
  if (C->project.windows.len())
    C->project.windows[0]->ToggleConsole();
}

void ToggleConcole_invoke(Seance* C, Operator* op) {
  ToggleConcole_ecec(C, op);
  op->state = OpState::FINISHED;
}

// Checks if operator can be inveked
bool ToggleConcole_poll(Seance* C, Operator* op) {
  return true;
}

void ToggleConcole_create(Seance* C, Operator* op) {
  op->idname = "Toggle Console";
  op->Poll = ToggleConcole_poll;
  op->Invoke = ToggleConcole_invoke;

  op->state = OpState::NONE;
}

// -----------  Console Toggle Operator end ----------------------- //

// -----------  Window Resize Operator ----------------------- //

struct WinResizeData {
  bool top = false;
  bool right = false;
  bool bottom = false;
  bool left = false;
  Window* win = nullptr;
};


void WindowResize_ecec(Seance* C, Operator* op) {}

void WindowResize_invoke(Seance* C, Operator* op) {
  WinResizeData* data = (WinResizeData*)op->CustomData;

  vec2<SCR_UINT>* crsr = &data->win->user_inputs.Cursor;
  Rect<SCR_UINT> rect;
  data->win->getWinRect(rect);

  SCR_UINT winw = rect.width();
  SCR_UINT winh = -rect.height();

  float fracx = winw / 3.f;
  float fracy = winh / 3.f;

  data->top = crsr->y < fracy;
  data->right = crsr->x > fracx * 2.f;
  data->bottom = crsr->y > fracy * 2.f;
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

  int dx = -(data->win->user_inputs.PrevCursor.x - data->win->user_inputs.Cursor.x);
  int dy = -(data->win->user_inputs.PrevCursor.y - data->win->user_inputs.Cursor.y);

  Rect<SCR_UINT> rect;
  data->win->getWinRect(rect);

  if (data->top) {
    rect.v1.y += dy;
    rect.v2.y += dy;
    data->win->user_inputs.Cursor.y -= dy;
  }
  if (data->right) {
    rect.v2.x += dx;
    rect.v3.x += dx;
  }
  if (data->bottom) {
    rect.v0.y += dy;
    rect.v3.y += dy;
  }
  if (data->left) {
    rect.v0.x += dx;
    rect.v1.x += dx;
    data->win->user_inputs.Cursor.x -= dx;
  }

  data->win->setWinRect(rect);
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

// -----------  Window Resize Operator end ----------------------- //

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
  if (event && event->idname == "FINISH") {
    op->state = OpState::FINISHED;
    return;
  }

  Window* data = (Window*)op->CustomData;
  UserInputs* usin = &data->user_inputs;

  int dx = -(usin->PrevCursor.x - usin->Cursor.x);
  int dy = -(usin->PrevCursor.y - usin->Cursor.y);

  Rect<SCR_UINT> rect;
  data->getWinRect(rect);

  rect.v1.x += dx;
  rect.v1.y += dy;

  rect.v3.x += dx;
  rect.v3.y += dy;

  data->setWinRect(rect);

  usin->Cursor.x -= dx;
  usin->Cursor.y -= dy;
}

void WindowDrag_create(Seance* C, Operator* op) {
  op->idname = "Move Window";
  op->Poll = WindowDrag_poll;
  op->Modal = WindowDrag_modal;
  op->Invoke = WindowDrag_invoke;

  op->state = OpState::NONE;
}

// -----------  Window Drag Operator end ----------------------- //

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
}
