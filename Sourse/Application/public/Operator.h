#pragma once
#include "Property.h"
//#include "LinkedList.h"
#define MAX_OP_EVENT_IDNAME 10
#define MAX_OPERATOR_NAME_LENGTH 20



struct Operator {
  char idname[MAX_OPERATOR_NAME_LENGTH];
  Properties Props;
  //OpState State;
  class List<char[MAX_OP_EVENT_IDNAME]> op_events;

  void (*Invoke)(struct Seance* C, Operator* op, char* event_idname) = nullptr;
  void (*Execute)(struct Seance* C, Operator* op) = nullptr;
  void (*Modal)(struct Seance* C, Operator* op) = nullptr;
  void (*Poll)(struct Seance* C, Operator* op) = nullptr;

  void* CustomData = nullptr;
};

struct ExecComand {
  ExecComand(Operator* op, char* event_idname);
  Operator* op;
  char* event_idname;
};

void OpsInit(struct Seance* C);