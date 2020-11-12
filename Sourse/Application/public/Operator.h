#pragma once
#include "Property.h"

#define MAX_OP_EVENT_IDNAME 10
#define MAX_OPERATOR_NAME_LENGTH 20

enum class OpEventState {
  NONE = 0,
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

struct ModalEvent {
  char idname[MAX_OP_EVENT_IDNAME];
};

struct Operator {
  // OP idname
  char idname[MAX_OPERATOR_NAME_LENGTH];

  // Current state of op
  OpState state;

  // Modal Map event idnames
  class List<ModalEvent> modal_events;

  // Arguments
  Properties Props;

  // Callbacks
  void (*Invoke)(struct Seance* C, Operator* op) = nullptr;
  void (*Execute)(struct Seance* C, Operator* op) = nullptr;
  void (*Modal)(struct Seance* C, Operator* op, ModalEvent* modal_ev) = nullptr;
  void (*Poll)(struct Seance* C, Operator* op) = nullptr;

  // Modal cache data
  void* CustomData = nullptr;
};

struct ExecComand {
  ExecComand(Operator* op, OpEventState op_event, ModalEvent* modal_event);

  Operator* op;
  OpEventState op_event;
  ModalEvent* modal_event;
};

void initOps(struct Seance* C);