#pragma once
#include "Property.h"
//#include "LinkedList.h"
#define MAX_OP_EVENT_IDNAME 10
#define MAX_OPERATOR_NAME_LENGTH 20

struct ExecComand {
  ExecComand(Operator* op, char* event_idname);
  Operator* op;
  char* event_idname;
};

struct Operator {
  char idname[MAX_OPERATOR_NAME_LENGTH];
  Properties Props;
  //OpState State;
  class List<char[MAX_OP_EVENT_IDNAME]> op_events;

  void (*Invoke)(class Seance* C, Operator* op, char* event_idname) = nullptr;
  void (*Execute)(class Seance* C, Operator* op) = nullptr;
  void (*Modal)(class Seance* C, Operator* op) = nullptr;
  void (*Poll)(class Seance* C, Operator* op) = nullptr;

  void* CustomData = nullptr;
};

void OpsInit(class Context* C);