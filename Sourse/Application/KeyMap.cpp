
#include "public/KeyMap.h"
//#include <inttypes.h>
#include <fstream>
//#include <iostream>
#include <sstream>
//#include "public/Print.h"
#include "String.h"

using namespace std;

#define KB_START '<'
#define KB_END '>'

#define MMPITEM_START '['
#define MMPITEM_END ']'

#define IDNAME_BOUD '"'

#define MMAP_START '['
#define MMAP_END ']'

#define COND_START '{'
#define COND_END '}'

#define CONDS_START '('
#define CONDS_END ')'

// ---------------------------- //

inline Range find_idname(Str* str, const char start_kw, const char end_kw, Range bnds) {
  return Range(str->find(start_kw, bnds), str->rfind(end_kw, bnds));
}

inline Range find_idname_straigt(Str* str, const char start_kw, const char end_kw, Range bnds) {
  str_idx p1 = str->find(start_kw, bnds) + 1;
  str_idx p2 = str->find(end_kw, Range(p1 + 1, bnds.end + 1)) - 1;

  if (p2 > bnds.end) {
    return Range(-1, -1);
  }
  return Range(p1, p2);
}

// ---------------------------- //

Input::Input() {}

Input::Input(const char* idname, InputState state) {
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

COpBindings::COpBindings() {
  op_ptr = nullptr;
  thread_ptr = nullptr;
}

COpBindings::~COpBindings() {
  modal_keymap.free();
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
        OpThread* new_tread = NEW_DBG(OpThread) OpThread(op, OpEventState::INVOKE, nullptr);
        op_map->thread_ptr = new_tread;
        op_threads->add(new_tread);
      }
    }
  }
}

// ---------------------------- //

bool CKeyCondition::Compile(UserInputs* usins, Str* str, Range& bnds) {

  str_idx cursor = bnds.strt;

  // find input name
  Range inp_id_bnds = find_idname_straigt(str, IDNAME_BOUD, IDNAME_BOUD, Range(cursor, bnds.end));
  if (!inp_id_bnds.valid()) {
    return false;
  }
  Str input_idname;
  input_idname.coppy(*str, inp_id_bnds);

  // move cursor
  cursor = inp_id_bnds.end + 2;

  // find triger state name
  Range trigger_id_bnds = find_idname_straigt(str, IDNAME_BOUD, IDNAME_BOUD, Range(cursor, bnds.end));
  if (!trigger_id_bnds.valid()) {
    return false;
  }
  Str triger_state;
  triger_state.coppy(*str, trigger_id_bnds);

  // map into existing inputs
  Input* iter = &usins->SYS_DESTROY_COMMAND;
  while (!(iter->idname == "END_OF_INPUTS")) {
    if (iter->idname == input_idname) {
      input_ptr = &(iter->state);

      switch (triger_state[0]) {
        case 'H':
          this->trigger_state = InputState::HOLD;
          break;

        case 'P':
          this->trigger_state = InputState::PRESSED;
          break;

        case 'R':
          this->trigger_state = InputState::RELEASED;
          break;

        default:
          return false;
      }

      return true;
    }
    iter += 1;
  }
  return false;
}

bool CShortcut::Compile(UserInputs* usins, Str* str, Range& bnds) {

  // Init conditions
  str_idx cursor = bnds.strt;
  do {

    // find condition item item
    Range cond_bnds = find_idname_straigt(str, COND_START, COND_END, Range(cursor, bnds.end));
    if (!cond_bnds.valid()) {
      break;
    }

    cursor = cond_bnds.strt;

    CKeyCondition* cond = NEW_DBG(CKeyCondition) CKeyCondition();

    if (cond->Compile(usins, str, cond_bnds)) {
      this->conditions.add(cond);
    } else {

      DELETE_DBG(CKeyCondition, cond);
    }
    cursor = cond_bnds.end + 2;


  } while (true);

  return (bool)this->conditions.len();
}

CShortcut::~CShortcut() {
  conditions.free();
}

bool COpBindings::Compile(List<Operator>* ops, UserInputs* usins, Str* str, Range bnds) {

  this->op_ptr = nullptr;

  str_idx cursor = bnds.strt;

  // find op identifier in keymap
  Range op_name = find_idname_straigt(str, IDNAME_BOUD, IDNAME_BOUD, Range(cursor, bnds.end));
  if (!op_name.valid()) {
    /*
    post_MSG(CMSGType::WARNING,
             "Compiling KeyMap: Op Identifier Not Found. Corrupted KeyMap file at " + bnds.strt);
    */
    return false;
  }
  cursor = op_name.strt;

  // find op in prefs
  FOREACH_NODE(Operator, ops, op_node) {

    if (op_node->Data->idname.match(Range(0, op_node->Data->idname.len() - 1), *str, op_name)) {
      this->op_ptr = op_node->Data;
      break;
    }
  }
  if (!op_ptr) {
    Str warning_op;
    warning_op.coppy(*str, op_name);
    //post_MSG(CMSGType::WARNING, "Compiling KeyMap: Couldn't find spicified operator: " + warning_op);
    return false;
  }

  // find invoke
  Range invk = find_idname_straigt(str, CONDS_START, CONDS_END, Range(cursor, bnds.end));
  if (!invk.valid()) {
    /*
    post_MSG(CMSGType::WARNING,
             "Compiling KeyMap: Invoke Identifier Not Found. Corrupted KeyMap file at " + bnds.strt);
    */
    return false;
  }
  // init invoke shortcut
  if (!(invoke.Compile(usins, str, invk))) {
    return false;
  }


  Range modalm = find_idname(str, MMAP_START, MMAP_END, bnds);
  if (!invk.valid()) {
    return false;
  }

  // Init modal items
  str_idx mmp_cursor = modalm.strt;
  do {

    // find modal map item
    Range mmp_item = find_idname_straigt(str, MMPITEM_START, MMPITEM_END, Range(mmp_cursor, modalm.end));
    if (!mmp_item.valid()) {
      break;
    }

    mmp_cursor = mmp_item.strt;


    CShortcut* modal_shcut = NEW_DBG(CShortcut) CShortcut();

    // find event identifier
    Range modal_event_name =
        find_idname_straigt(str, IDNAME_BOUD, IDNAME_BOUD, Range(mmp_cursor, mmp_item.end));
    if (!modal_event_name.valid()) {
      continue;
    }
    modal_shcut->modal_event.idname.coppy(*str, modal_event_name);


    // find conditions
    Range conditions = find_idname_straigt(str, CONDS_START, CONDS_END, Range(mmp_cursor, mmp_item.end));
    if (!conditions.valid()) {
      continue;
    }

    // init conditions
    if (modal_shcut->Compile(usins, str, conditions)) {
      this->modal_keymap.add(modal_shcut);
    } else {
      DELETE_DBG(CShortcut, modal_shcut);
    }

    mmp_cursor = mmp_item.end;

  } while (true);

  return true;
}

void CompiledKeyMap::Compile(List<Operator>* ops, UserInputs* usins, Str* filipath) {

  // Loading File into string
  ifstream keymap_file;
  keymap_file.open(string(filipath->str, filipath->len()));
  if (!keymap_file.is_open()) {
    //post_MSG(CMSGType::ERRORtype, "Compiling KeyMap: Couldn't load the keymap");
  }

  stringstream strStream;
  strStream << keymap_file.rdbuf();
  Str kmap = Str(strStream.str().c_str());
  //Str kmap;
  //string s = strStream.str();

  // Reading KeyMap
  str_idx cursor = 0;
  do {

    Range bds = find_idname_straigt(&kmap, KB_START, KB_END, Range(cursor, kmap.len()));
    if (!bds.valid()) {
      break;
    }

    cursor = bds.strt;

    COpBindings* new_bdgs = NEW_DBG(COpBindings) COpBindings();
    if (new_bdgs->Compile(ops, usins, &kmap, bds)) {
      this->op_bindings.add(new_bdgs);
    } else {
      DELETE_DBG(COpBindings, new_bdgs);
    }

    cursor = bds.end;

  } while (true);

  keymap_file.close();
}

CompiledKeyMap::~CompiledKeyMap() {
  op_bindings.free();
}
