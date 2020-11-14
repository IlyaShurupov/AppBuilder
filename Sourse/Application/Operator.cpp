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
}
