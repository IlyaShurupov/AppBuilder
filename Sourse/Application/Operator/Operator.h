#pragma once
#include "Property.h"

enum class OpEvState {
  EXECUTE,
  INVOKE,
  MODAL_EVENT,
};

enum class OpState {
  NONE = 0,
  RUNNING_MODAL,
  FINISHED,
  CANCELED,
};

enum class ThreadState {
  RUNNING,
  CLOSED,
  DENIED,
};

struct OpArg {
  Str idname;

  OpArg(){};
  OpArg(Str idname) { this->idname = idname; }
  OpArg(char* idname) { this->idname = idname; }
};

class Operator {
 public:
  // OP idname
  struct Str id;

  bool continues;

  // Current state of op
  OpState state = OpState::NONE;

  // Run Time Arguments
  class List<OpArg> rtargs;

  // Callbacks
  virtual void Invoke(struct Seance* C) { Execute(C); }
  virtual void Execute(struct Seance* C) { state = OpState::CANCELED; }
  virtual void Modal(struct Seance* C, OpArg* rtarg) {}
  virtual bool Poll(struct Seance* C) { return false; }
};


struct OpThread {
  OpThread(Operator* op, OpEvState op_event, OpArg* modalevent);

  ThreadState state;
  Operator* op;
  OpEvState op_event;
  OpArg* modalevent;
};

