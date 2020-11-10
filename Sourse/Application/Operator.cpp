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

void ObjectRotate_modal(Context* C, Operator* op) {
  AppEvent* enent = &C->event;
  OPDATA_FROM_OP(ObjectRotateCData, OpData);

  if (enent->B == KeyState::RELEASED) {
    RET_FINISHED(op);
  }
}

void ObjectRotate_ecec(Context* C, Operator* op) {}

void ObjectRotate_invoke(Context* C, Operator* op) {
  OPDATA_FROM_OP(ObjectRotateCData, OpData);
  OpData->SomeVec.x = 3;
  op->State = OpState::RUNNING;
}

// Checks if operator should be inveked
// TODO:invent keymap
void ObjectRotate_poll(Context* C, Operator* op) {
  AppEvent* enent = &C->event;
  if (enent->B == KeyState::PRESSED) {
    op->State = OpState::INVOKE;
  }
}

void ObjectRotate_create(Context* C, Operator* op) {
  op->Poll = ObjectRotate_poll;
  op->Invoke = ObjectRotate_invoke;
  op->Modal = ObjectRotate_modal;

  op->State = OpState::FINISHED;
  op->CustomData = (void*)DBG_NEW ObjectRotateCData;
}

// -----------  test Operetor end ----------------------- //

void AddOperator(Context* C, void (*Create)(Context* C, Operator* op)) {
  Operator* op = DBG_NEW Operator;
  Create(C, op);
  C->operators.add(op);
}

void OpsInit(Context* C) { AddOperator(C, ObjectRotate_create); }