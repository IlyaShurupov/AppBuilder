#include "public/Operator.h"

#include "public/Print.h"
#include "public/Seance.h"
#define OPDATA_FROM_OP(Type, name) Type* name = (Type*)op->CustomData;
#define RET_FINISHED(op)         \
  op->State = OpState::FINISHED; \
  return;

// -----------  End Seance Operator ----------------------- //

void EndSeance_ecec(Seance* C, Operator* op) {
  // save &
  delete C;
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

Operator::~Operator() {
  if (CustomData) {
    delete CustomData;
  }
  modal_events.del();
}
