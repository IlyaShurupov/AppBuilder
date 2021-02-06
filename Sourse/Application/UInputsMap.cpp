
#include "public/UInputsMap.h"

Input::Input(const char* name) {
  idname = name;
}

bool Trigger::active() {
  return scope_conds.satisfies() && key_conds.satisfies();
}

void OPInterface::proc(List<OpThread>* queue) {
  if (op_thread && op_thread->state != ThreadState::RUNNING) {
    FREE(op_thread);
    op_thread = nullptr;
  }

  FOREACH(&triggers, Trigger, node) {
    Trigger* trigger = node->Data;
    if ((!op_thread && !trigger->runtime) || (op_thread && trigger->runtime)) {
      IF(!node->Data->active(), continue;);
      queue->add(NEW_DBG(OpThread) OpThread(target, OpEvState::INVOKE, &trigger->arg));
    }
  }
}

void KeyMap::evaluate(List<OpThread>* exec_queue) {
  FOREACH_DO(opiterfaces, OPInterface, iter->proc(exec_queue));
}

// ---------------------------- compile -------------------------------------------- //

InputState* input_state_find(Str* string, void* uinputs) {
  UInputs* usins = (UInputs*)uinputs;
  Input* input = &usins->SYS_DESTROY_COMMAND;

  while (!(input->idname == "_UIEND_")) {
    if (input->idname == *string) {
      return &input->state;
    }
  }

  return nullptr;
}

UIIstate* uii_state_find(Str* string, void* uiroot) {
  
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

  runtime = db->find("OnlyRunTime")->boolean;
  arg.idname = db->find("Argument")->string;
  DataBlock* conditionsdb = db->find("Conditions");
  
  key_conds.Compile(conditionsdb->find("Keys"), uinputs);
  scope_conds.Compile(conditionsdb->find("Scopes"), root);
}


void OPInterface::Compile(DataBlock* db, List<Operator>* ops, UInputs* uinputs, UIItem* root) {
  target = find_op(ops, &db->find("Operator")->string);

  DataBlock* triggersdb = db->find("Triggers");

  FOREACH(&triggersdb->list, DataBlock, t_node) {

    Trigger* trigger = NEW_DBG(Trigger) Trigger();
    trigger->Compile(t_node->Data, uinputs, root);
    triggers.add(trigger);
  }
}

void KeyMap::Compile(DataBlock* db, List<Operator>* ops, UInputs* uinputs, UIItem* root) {

  DataBlock* kmdb = db->find("KeyMap");

  FOREACH(&db->list, DataBlock, opi_node) {
    
    OPInterface* opintrface = NEW_DBG(OPInterface) OPInterface();
    opintrface->Compile(opi_node->Data, ops, uinputs, root);
    opiterfaces.add(opintrface);
  }
}
