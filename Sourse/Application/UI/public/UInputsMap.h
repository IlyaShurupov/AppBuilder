
#pragma once

#include "LinkedList.h"
#include "Strings.h"
#include "..//..//public/Operator.h"

struct UInputs;
struct DataBlock;
struct UIItem;

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
      if (node->Data->met()) {
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
      Cond<type>* cond = NEW_DBG(Cond<type>) Cond<type>();
      cond->target = find_target(&node->Data->find("Name")->string, sourse);
      cond->trigger_state = to_state(&node->Data->find("State")->string);
      conds.add(cond);
    }
  }

  ~Conditions() { conds.del(); }
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

  struct Operator* target = nullptr;
  OpThread* op_thread = nullptr;
  List<Trigger> triggers;

  void proc(List<OpThread>* queue);

  void Compile(DataBlock* db, List<Operator>* ops, UInputs* uinputs, UIItem* root);
  ~OPInterface();
};

struct KeyMap {

  List<OPInterface> opiterfaces;  

  void evaluate(List<OpThread>* exec_queue);

  void Compile(DataBlock* db, List<Operator>* ops, UInputs* uinputs, UIItem* root);
};
