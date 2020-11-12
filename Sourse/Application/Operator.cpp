#include "public/Operator.h"

#include "Print.h"
#include "public/Seance.h"
#define OPDATA_FROM_OP(Type, name) Type* name = (Type*)op->CustomData;
#define RET_FINISHED(op)         \
  op->State = OpState::FINISHED; \
  return;

// -----------  test Operetor ----------------------- //

void InitSeance_modal(Seance* C, Operator* op, ModalEvent* modal_ev) {

}

void InitSeance_ecec(Seance* C, Operator* op) {}

void InitSeance_invoke(Seance* C, Operator* op) {

}

// Checks if operator should be inveked
// TODO:invent keymap
void InitSeance_poll(Seance* C, Operator* op) {

}

void DummySeance_create(Seance* C, Operator* op) {
  //op->idname = name;
  op->Poll = InitSeance_poll;
  op->Invoke = InitSeance_invoke;

  op->state = OpState::NONE;
}

// -----------  test Operetor end ----------------------- //

void AddOperator(Seance* C, void (*Create)(Seance* C, Operator* op)) {
  Operator* op = DBG_NEW Operator;
  Create(C, op);
  C->prefferences.operators.add(op);
}

ExecComand::ExecComand(Operator* op, OpEventState op_event,
                       ModalEvent* modal_event)
    : op(op), modal_event(modal_event), op_event(op_event) {}

void initOps(Seance* C) {
  //AddOperator(C, DummySeance_create);
  
}
