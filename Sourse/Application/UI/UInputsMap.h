
#pragma once

#include "Core/Operator.h"
#include "Containers/List.h"
#include "Strings.h"

struct UInputs;
struct UInputs;
struct DataBlock;
class UIItem;
struct Operators;

enum class InputState {
  NONE = 0,
  HOLD,
  PRESSED,
  RELEASED,
};

struct Input {
  Input(const char* name);

  struct Str idname;
  InputState state = InputState::NONE;
};

template <class type>
struct Cond {
  type* target = nullptr;
  type trigger_state;
  bool met() { return *target == trigger_state; }
};

template <class type>
struct Conditions {

  bool any = false;
  List<Cond<type>> conds;

  bool satisfies() {
    FOREACH(&conds, Cond<type>, node) {
      if (node->met()) {
        if (any)
          return true;
        continue;
      }
      return false;
    }
    return true;
  }

  type* (*find_target)(Str* string, void* sourse) = nullptr;
  type (*to_state)(Str* string) = nullptr;

  void Compile(DataBlock* db, void* sourse) {

    DataBlock* condlistdb = nullptr;
    any = db->find("Logic")->string == "ANY";
    List<DataBlock>* conds_list = &db->find("List")->list;
    FOREACH(conds_list, DataBlock, node) {
      Cond<type>* cond = NEW(Cond<type>)();
      cond->target = find_target(&node->find("Name")->string, sourse);
      cond->trigger_state = to_state(&node->find("State")->string);
      conds.PushBack(cond);
    }
  }

  ~Conditions() {  }
};

struct Trigger {

  bool runtime = false;
  struct OpArg arg;

  Conditions<enum class UIIstate> scope_conds;
  Conditions<enum class InputState> key_conds;

  bool active();

  void Compile(DataBlock* db, UInputs* uinputs, UIItem* root);
};

struct OPInterface {

  class Operator* target = nullptr;
  OpThread* op_thread = nullptr;
  List<Trigger> triggers;

  void proc(List<OpThread>* queue);

  void Compile(DataBlock* db, Operators* ops, UInputs* uinputs, UIItem* root);
  ~OPInterface();
};

struct KeyMap {

  KeyMap();
  ~KeyMap();

  List<OPInterface> opiterfaces;
  UInputs* uinputs;

  void evaluate(List<OpThread>* exec_queue);

  void Compile(DataBlock* db, Operators* ops, UIItem* root);
};
