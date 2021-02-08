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

struct Operator {
  // OP idname
  struct Str idname;

  // Current state of op
  OpState state = OpState::NONE;

  // Modal Map event idnames
  class List<OpArg> modal_events;

  // Arguments
  Properties Props;

  // Callbacks
  void (*Invoke)(struct Seance* C, Operator* op) = nullptr;
  void (*Execute)(struct Seance* C, Operator* op) = nullptr;
  void (*Modal)(struct Seance* C, Operator* op, OpArg* modal_ev) = nullptr;
  bool (*Poll)(struct Seance* C, Operator* op) = nullptr;

  // Modal cache data
  void* CustomData = nullptr;

  ~Operator();
};

struct OpThread {
  OpThread(Operator* op, OpEvState op_event, OpArg* modalevent);

  ThreadState state;
  Operator* op;
  OpEvState op_event;
  OpArg* modalevent;
};

void initOps(struct Seance* C);
Operator* find_op(List<Operator>* operators, Str* op_idname);