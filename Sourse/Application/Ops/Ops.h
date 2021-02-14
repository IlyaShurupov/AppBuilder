#pragma once

#include "LinkedList.h"

struct Operators {
  
  Operators();
  ~Operators();

  List<class Operator> ops;
  Operator* find(struct Str* id);
};
