#include "public/Operator.h"

#include "public/Context.h"
#define OPDATA_FROM_OP(Type, name) Type* name = (Type*)op->CustomData;
#define RET_FINISHED(op)         \
  op->State = OpState::FINISHED; \
  return;

// -----------  test Operetor ----------------------- //

#include "Print.h"

typedef struct ObjectRotateCData {
  Vec3f SomeVec;
} ObjectRotateCData;

void ObjectRotate_modal(Seance* C, Operator* op) {
  // UserInputs* enent = &C->event;
  OPDATA_FROM_OP(ObjectRotateCData, OpData);

  // if (enent->B == KeyState::RELEASED) {
  // RET_FINISHED(op);
  //}
}

void ObjectRotate_ecec(Seance* C, Operator* op) {}

void ObjectRotate_invoke(Seance* C, Operator* op, char* event_idname) {
  // OPDATA_FROM_OP(ObjectRotateCData, OpData);
  // OpData->SomeVec.x = 3;
  // op->State = OpState::RUNNING;
}

// Checks if operator should be inveked
// TODO:invent keymap
void ObjectRotate_poll(Seance* C, Operator* op) {
  // UserInputs* enent = &C->event;
  // if (enent->B == KeyState::PRESSED) {
  // op->State = OpState::INVOKE;
  //}
}

void ObjectRotate_create(Seance* C, Operator* op) {
  op->Poll = ObjectRotate_poll;
  op->Invoke = ObjectRotate_invoke;
  op->Modal = ObjectRotate_modal;

  // op->State = OpState::FINISHED;
  // op->CustomData = (void*)DBG_NEW ObjectRotateCData;
}

// -----------  test Operetor end ----------------------- //

void AddOperator(Seance* C, void (*Create)(Seance* C, Operator* op)) {
  Operator* op = DBG_NEW Operator;
  Create(C, op);
  // C->operators.add(op);
}

void OpsInit(Seance* C) { AddOperator(C, ObjectRotate_create); }

ExecComand::ExecComand(Operator* op, char* event_idname)
    : op(), event_idname() {}