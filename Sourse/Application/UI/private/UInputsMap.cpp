
#include "UI/UInputsMap.h"
#include "Core/Operator.h"
#include "UI/UInterface.h"
#include "UI/UInputs.h"
#include "IO/Parser.h"
#include "Ops/Ops.h"

Input::Input(const char* name) {
  idname = name;
}

bool Trigger::active() {
  return scope_conds.satisfies() && key_conds.satisfies();
}

void OPInterface::proc(List<OpThread>* queue) {
  if (op_thread && op_thread->state != ThreadState::RUNNING) {
    DEL(OpThread, op_thread);
    op_thread = nullptr;
  }

  FOREACH(&triggers, Trigger, node) {
    Trigger* trigger = node.Data();
    if ((!op_thread && !trigger->runtime) || (op_thread && trigger->runtime)) {
      if (node->active()) {

        if (node->runtime) {
          op_thread->modalevent = &node->arg;
        } else {
          op_thread = NEW(OpThread)(target, OpEvState::INVOKE, nullptr);
          queue->PushBack(op_thread);
        }
      }
    }
  }
}

OPInterface::~OPInterface() {
}

void KeyMap::evaluate(List<OpThread>* exec_queue) {
  FOREACH(&opiterfaces, OPInterface, opiterface) {
    opiterface->proc(exec_queue);
  }
}

KeyMap::KeyMap() {
  uinputs = NEW(UInputs)();
}

KeyMap::~KeyMap() {
  DEALLOC(uinputs);
}

// ---------------------------- compile -------------------------------------------- //

InputState* input_state_find(Str* string, void* uinputs) {
  UInputs* usins = (UInputs*)uinputs;
  Input* input = &usins->K0;

  while (!(input->idname == "_UIEND_")) {
    if (input->idname == *string) {
      return &input->state;
    }
    input = input + 1;
  }

  return nullptr;
}

UIIstate* uii_state_find(Str* string, void* uiroot) {
  UIItem* root = (UIItem*)uiroot;
  return &root->find(string)->state;
}

InputState input_state_from_str(Str* string) {
  if (*string == "HOLD") {
    return InputState::HOLD;
  } 
  if (*string == "NONE") {
    return InputState::NONE;
  } 
  if (*string == "PRESSED") {
    return InputState::PRESSED;
  }
  return InputState::RELEASED;
}

UIIstate uii_state_from_str(Str* string) {
  if (*string == "NONE") {
    return UIIstate::NONE;
  }
  if (*string == "INSIDE") {
    return UIIstate::INSIDE;
  }
  if (*string == "LEAVED") {
    return UIIstate::LEAVED;
  }
  return UIIstate::ENTERED;
}

void Trigger::Compile(DataBlock* db, UInputs* uinputs, UIItem* root) {

  DataBlock* conditionsdb = db->find("Conditions");
  runtime = db->find("Runtime")->boolean;
  arg.idname = db->find("Argument")->string;
  
  DataBlock* keycondsdb = conditionsdb->find("Keys");
  if (keycondsdb) {
    key_conds.find_target = input_state_find;
    key_conds.to_state = input_state_from_str;
    key_conds.Compile(keycondsdb, uinputs);
  }

  DataBlock* scopecondsdb = conditionsdb->find("Scopes");
  if (scopecondsdb) {
    scope_conds.find_target = uii_state_find;
    scope_conds.to_state = uii_state_from_str;
    scope_conds.Compile(scopecondsdb, root);
  }
}


void OPInterface::Compile(DataBlock* db, Operators* ops, UInputs* uinputs, UIItem* root) {
  target = ops->find(&db->find("Operator")->string);

  DataBlock* triggersdb = db->find("Triggers");

  FOREACH(&triggersdb->list, DataBlock, t_node) {

    Trigger* trigger = NEW(Trigger)();
    trigger->Compile(t_node.Data(), uinputs, root);
    triggers.PushBack(trigger);
  }
}

void KeyMap::Compile(DataBlock* db, Operators* ops, UIItem* root) {

  DataBlock* kmdb = db->find("KeyMap");

  FOREACH(&kmdb->list, DataBlock, opi_node) {
    
    OPInterface* opintrface = NEW(OPInterface)();
    opintrface->Compile(opi_node.Data(), ops, uinputs, root);
    opiterfaces.PushBack(opintrface);
  }
}
