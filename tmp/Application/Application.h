
# pragma once

#include "Types.h"

class UIItem;
class Operator;
struct Operators;


class Application {

  Timer timer;
  struct Seance* C;

 public:
  Application(uint2 p_fps);

  void AddOperator(Operator* op);
  void LoadConfiguration(Str* projpath, UIItem* (*UIIFromStr)(Str* id, Operators* ops, DataBlock* paramsdb, DataBlock* uiidb));
  void Launch();
};
