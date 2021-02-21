#include "Operator/Operator.h"

#include "Data/Seance.h"
#include "Types.h"

OpThread::OpThread(Operator* target, OpEvState cmd, OpArg* rtarg) {
  op_event = cmd; 
  modalevent = rtarg;
  op = target;
  state = ThreadState::RUNNING;
}