
#include "public/KeyMap.h"

bool CKeyCondition::IsTrue() {
  return (*this->input_ptr == this->trigger_state);
}

bool CShortcut::IsShortcut() {
  FOREACH_STACK(cond, CKeyCondition, conditions.first) {
    if (!cond->data->IsTrue()) {
      return false;
    }
  }
  return true;
}

ModalEvent* COpBindings::IsModalEvent() {
  FOREACH_STACK(shortcut, CShortcut, modal_keymap.first) {
    if (shortcut->data->IsShortcut()) {
      return &shortcut->data->modal_event;
    }
  }
  return nullptr;
}

bool COpBindings::IsInvoked() {
  return invoke.IsShortcut();
}

void CompiledKeyMap::ProcEvents(List<ExecComand>* exec_queue) {

  FOREACH_STACK(op_map_node, COpBindings, op_bindings.first) {
    COpBindings* op_map = op_map_node->data;
    Operator* op = op_map->op_ptr;

    if (op->state == OpState::RUNNING_MODAL) {
      ModalEvent* mev = op_map->IsModalEvent();
      if (mev) {
        exec_queue->add(new ExecComand(op, OpEventState::MODAL_EVENT, mev));
      }
    } else {
      if (op_map->IsInvoked()) {
        exec_queue->add(new ExecComand(op, OpEventState::INVOKE, nullptr));
      }
    }
  }

}

bool CKeyCondition::Compile(UserInputs* usr_inputs, KeyCondition* usr_cond) {
  for (int i = 0; i < usr_inputs->length; i++) {
    if ((&usr_inputs->A)[i].idname == usr_cond->input_idname) {
      input_ptr = &(&usr_inputs->A)[i].state;
      trigger_state = usr_cond->trigger_state;
      return true;
    }
  }
  return false;
}

bool CShortcut::Compile(UserInputs* usr_inputs, Shortcut* shortcut) {
  this->modal_event = shortcut->modal_event;

  FOREACH_STACK(usr_cond, KeyCondition, shortcut->conditions.first) {
    CKeyCondition* key_cond = new CKeyCondition();

    if (key_cond->Compile(usr_inputs, usr_cond->data)) {
      conditions.Add(*key_cond);
    }
  }
  return true;
}

bool COpBindings::Compile(char* operator_idname,
                          List<Operator>* operators,
                          UserInputs* usr_inputs,
                          OpBindings* user_op_bindings) {
  op_ptr = nullptr;
  FOREACH_NODE(Operator, operators, op_node) {
    if (op_node->Data->idname == operator_idname) {
      op_ptr = op_node->Data;
      break;
    }
  }
  if (op_ptr) {
    invoke.Compile(usr_inputs, &user_op_bindings->invoke);

    FOREACH_STACK(usr_mm_node, Shortcut, user_op_bindings->modal_keymap.first) {
      CShortcut* c_shortcut = new CShortcut();

      if (c_shortcut->Compile(usr_inputs, usr_mm_node->data)) {
        modal_keymap.Add(*c_shortcut);
      }
    }

    return true;
  }
  return false;
}

void CompiledKeyMap::Compile(List<Operator>* operators,
                             UserKeyMap* user_map,
                             UserInputs* usr_inputs) {
  FOREACH_NODE(OpBindings, (&user_map->map), user_op_map_node) {
    OpBindings* user_op_bindings = user_op_map_node->Data;

    COpBindings* new_op_b = new COpBindings();
    new_op_b->Compile(user_op_bindings->op_name, operators, usr_inputs,
                      user_op_bindings);

    if (new_op_b) {
      op_bindings.Add(*new_op_b);
    }
  }
}


