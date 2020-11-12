
#include "public/KeyMap.h"



bool KeyCondition::IsMet(UserInputs* inputs) {
  return inputs->map.find(input_idkey)->second == trigger_state;
}

bool Shortcut::IsShortcut(UserInputs* inputs) {
  FOREACH_STACK(node, KeyCondition, conditions.first) {
    if (!node->data->IsMet(inputs)) {
      return false;
    }
  }
  return true;
}

ModalEvent* OperatorBindings::IsModalEvent(UserInputs* inputs) {
  FOREACH_STACK(node, Shortcut, modal_keymap.first) {
    if (!node->data->IsShortcut(inputs)) {
      return &(node->data->modal_event);
    }
  }
  return nullptr;
}

bool OperatorBindings::IsInvoked(UserInputs* inputs) { 
  return invoke.IsShortcut(inputs);
}

OperatorBindings::OperatorBindings(char* operator_idname,
                                   List<Operator>* operators) {
  FOREACH_NODE(Operator, operators, op_node) {
    if (op_node->Data->idname == operator_idname) {
      op_ptr = op_node->Data;
    }
  }
  op_ptr = nullptr;
}

void KeyMap::add(Operator* op, OperatorBindings* key_bidings) {
  map.insert(std::pair<char*, OperatorBindings*>(op->idname, key_bidings));
}
