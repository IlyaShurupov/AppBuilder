#include "Operator/Operator.h"

#include "Data/Seance.h"
#include "Types.h"

OpThread::OpThread(Operator* target, OpEvState cmd, OpArg* rtarg) {
  op_event = cmd;
  modalevent = rtarg;
  op = target;
  state = ThreadState::RUNNING;
}

Operator* Operators::find(Str* id) {
  Operator* target = nullptr;
  Range bnds = Range(0, id->len());
  FOREACH(&ops, Operator, node) {
    if (node->id.match(bnds, *id, bnds)) {
      target = node.Data();
      break;
    }
  }
  return target;
}

Operators::Operators() {}
Operators::~Operators() {}