#pragma once
#include "Property.h"

enum class OpState { RUNNING, FINISHED, CANCELED, EXECUTE, INVOKE };

struct Operator {
  Properties Props;
  OpState State;

  void (*Invoke)(class Context* C, Operator* op) = nullptr;
  void (*Execute)(class Context* C, Operator* op) = nullptr;
  void (*Modal)(class Context* C, Operator* op) = nullptr;
  void (*Poll)(class Context* C, Operator* op) = nullptr;

  void* CustomData = nullptr;
};

void OpsInit(class Context* C);