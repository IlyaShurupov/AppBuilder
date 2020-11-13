#include "public/Operator.h"

#include "Print.h"
#include "public/Seance.h"
#define OPDATA_FROM_OP(Type, name) Type* name = (Type*)op->CustomData;
#define RET_FINISHED(op)         \
  op->State = OpState::FINISHED; \
  return;

// -----------  test Operetor ----------------------- //

void InitSeance_modal(Seance* C, Operator* op, ModalEvent* modal_ev) {
  if (modal_ev && modal_ev->idname == "CANCEL") {
    op->state = OpState::CANCELED;
    printf("CANCEL\n");
    return;
  }
  printf("RUNNING MODAL\n");
}

void InitSeance_ecec(Seance* C, Operator* op) {}

void InitSeance_invoke(Seance* C, Operator* op) {
  op->state = OpState::RUNNING_MODAL;
}

// Checks if operator can be inveked
bool InitSeance_poll(Seance* C, Operator* op) {
  if (true) {
    printf("Poll\n");
    return true;
  }
}

void DummySeance_create(Seance* C, Operator* op) {
  op->idname = "dummy_op";
  op->Poll = InitSeance_poll;
  op->Invoke = InitSeance_invoke;
  op->Modal = InitSeance_modal;

  op->state = OpState::NONE;
}

// -----------  test Operetor end ----------------------- //

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
  AddOperator(C, DummySeance_create);
}
