
#include "public/KeyMap.h"

#include "public/Operator.h"

bool InputCondition::IsMet(UserInputs* inputs) {
  return inputs->map.find(input_idkey)->second == trigger_state;
}

bool OpEvent::IsMet(UserInputs* inputs) {
  FOREACH_STACK(node, InputCondition, conditions.first) {
    if (!node->data->IsMet(inputs)) {
      return false;
    }
  }
  return true;
}

char* OperatorBinfings::IsOPEvent(UserInputs* inputs) {
  FOREACH_STACK(node, OpEvent, op_events.first) {
    if (!node->data->IsMet(inputs)) {
      return node->data->op_event_idname;
    }
  }
  return nullptr;
}

OperatorBinfings::OperatorBinfings(char* operator_idname,
                                   List<class Operator>* operators) {
  FOREACH_NODE(class Operator, operators, op_node) {
    if (op_node->Data->idname == operator_idname) {
      op_ptr = op_node->Data;
    }
  }
  op_ptr = nullptr;
}

void KeyMap::add(Operator* op, OperatorBinfings* key_bidings) {
  map.insert(std::pair<char*, OperatorBinfings*>(op->idname, key_bidings));
}