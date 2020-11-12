
#include "public/KeyMap.h"

Input::Input() {}

Input::Input(std::string idname, InputState state) {
  this->idname = idname;
  this->state = state;
}

bool CKeyCondition::IsTrue() {
  return (*this->input_ptr == this->trigger_state);
}

bool CShortcut::IsShortcut() {
  FOREACH_STACK(CKeyCondition, (&conditions), cond) {
    if (!cond->data->IsTrue()) {
      return false;
    }
  }
  return true;
}

ModalEvent* COpBindings::IsModalEvent() {
  FOREACH_STACK(CShortcut, (&modal_keymap), shortcut) {
    if (shortcut->data->IsShortcut()) {
      return &shortcut->data->modal_event;
    }
  }
  return nullptr;
}

bool COpBindings::IsInvoked() {
  return invoke.IsShortcut();
}

void CompiledKeyMap::ProcEvents(List<OpThread>* op_threads) {
  FOREACH_STACK(COpBindings, (&op_bindings), op_map_node) {
    COpBindings* op_map = op_map_node->data;
    Operator* op = op_map->op_ptr;

    if (op->state == OpState::RUNNING_MODAL) {
      ModalEvent* mev = op_map->IsModalEvent();
      if (mev) {
        op_map->thread_ptr->modal_event = mev;
      }
    } else {
      if (op_map->IsInvoked()) {
        OpThread* new_tread = new OpThread(op, OpEventState::INVOKE, nullptr);
        op_map->thread_ptr = new_tread;
        op_threads->add(new_tread);
      }
    }
  }
}

bool CKeyCondition::Compile(UserInputs* usr_inputs, KeyCondition* usr_cond) {
  Input* iter = &usr_inputs->A;
  while (iter->idname != "END_OF_INPUTS") {
    if (iter->idname == usr_cond->input_idname) {
      input_ptr = &iter->state;
      trigger_state = usr_cond->trigger_state;
      return true;
    }
    iter += 1;
  }
  return false;
}

bool CShortcut::Compile(UserInputs* usr_inputs, Shortcut* shortcut) {
  this->modal_event = shortcut->modal_event;

  FOREACH_STACK(KeyCondition, (&shortcut->conditions), usr_cond) {
    CKeyCondition* key_cond = new CKeyCondition();

    if (key_cond->Compile(usr_inputs, usr_cond->data)) {
      conditions.add(key_cond);
    } else {
      delete key_cond;
    }
  }
  return true;
}

bool COpBindings::Compile(std::string* operator_idname,
                          List<Operator>* operators,
                          UserInputs* usr_inputs,
                          OpBindings* user_op_bindings) {
  op_ptr = nullptr;
  FOREACH_NODE(Operator, operators, op_node) {
    if (op_node->Data->idname == *operator_idname) {
      op_ptr = op_node->Data;
      break;
    }
  }
  if (op_ptr) {
    invoke.Compile(usr_inputs, &user_op_bindings->invoke);

    FOREACH_STACK(Shortcut, (&user_op_bindings->modal_keymap), usr_mm_node) {
      CShortcut* c_shortcut = new CShortcut();

      if (c_shortcut->Compile(usr_inputs, usr_mm_node->data)) {
        modal_keymap.add(c_shortcut);
      } else {
        delete c_shortcut;
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
    new_op_b->Compile(&user_op_bindings->op_name, operators, usr_inputs,
                      user_op_bindings);

    if (new_op_b) {
      op_bindings.add(new_op_b);
    }
  }
}

