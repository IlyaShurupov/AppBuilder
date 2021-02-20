#pragma once

#include "Containers/List.h"

struct Operators {
  
  Operators();
  ~Operators();

  List<class Operator> ops;
  Operator* find(struct Str* id);
};
