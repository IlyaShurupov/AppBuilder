
#include "public/KeyMap.h"
//#include <inttypes.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "public/Print.h"

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

string getExecutablePath() {

  char* exePath;
  if (_get_pgmptr(&exePath) != 0)
    exePath = "";
  return std::string(exePath);
}

inline Bounds find_idname(std::string* str, const char start_kw, const char end_kw, Bounds bnds) {
  return Bounds(str->find(start_kw, bnds.strt), str->rfind(end_kw, bnds.end));
}

inline Bounds find_idname_straigt(std::string* str, const char start_kw, const char end_kw, Bounds bnds) {
  strloc p1 = str->find(start_kw, bnds.strt) + 1;
  strloc p2 = str->find(end_kw, p1 + 1) - 1;

  if (p2 > bnds.end) {
    return Bounds(-1, -1);
  }
  return Bounds(p1, p2);
}

inline bool valid(Bounds& bds) {
  return (bds.strt > 0 && bds.end > 0);
}

bool strs_match(string* str1, string* str2, Bounds& bnds1, Bounds& bnds2) {
  if (bnds1.len() != bnds2.len()) {
    return false;
  }
  for (strloc i = 0; i < bnds1.len(); i++) {
    char c1 = str1->at(bnds1.strt + i);
    char c2 = str2->at(bnds2.strt + i);
    if (c1 != c2) {
      return false;
    }
  }
  return true;
}

void str_coppy(string* str, Bounds& in_str_from_to, string& out) {
  out.clear();
  for (strloc i = in_str_from_to.strt; i < in_str_from_to.end + 1; i++) {
    out.push_back(str->at(i));
  }
}

// ---------------------------- //


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

// ---------------------------- //

bool CKeyCondition::Compile(UserInputs* usins, string* str, Bounds& bnds) {

  strloc cursor = bnds.strt;

  // find input name
  Bounds inp_id_bnds = find_idname_straigt(str, IDNAME_BOUD, IDNAME_BOUD, Bounds(cursor, bnds.end));
  if (!valid(inp_id_bnds)) {
    return false;
  }
  string input_idname;
  str_coppy(str, inp_id_bnds, input_idname);

  // move cursor
  cursor = inp_id_bnds.end + 2;

  // find triger state name
  Bounds trigger_id_bnds = find_idname_straigt(str, IDNAME_BOUD, IDNAME_BOUD, Bounds(cursor, bnds.end));
  if (!valid(trigger_id_bnds)) {
    return false;
  }
  string triger_state;
  str_coppy(str, trigger_id_bnds, triger_state);

  // map into existing inputs
  Input* iter = &usins->SYS_DESTROY_COMMAND;
  while (iter->idname != "END_OF_INPUTS") {
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

bool CShortcut::Compile(UserInputs* usins, string* str, Bounds& bnds) {

  // Init conditions
  strloc cursor = bnds.strt;
  do {

    // find condition item item
    Bounds cond_bnds = find_idname_straigt(str, COND_START, COND_END, Bounds(cursor, bnds.end));
    if (!valid(cond_bnds)) {
      break;
    }

    cursor = cond_bnds.strt;

    CKeyCondition* cond = new CKeyCondition();

    if (cond->Compile(usins, str, cond_bnds)) {
      this->conditions.add(cond);
    } else {

      delete cond;
    }
    cursor = cond_bnds.end + 2;


  } while (true);

  return (bool)this->conditions.len();
}

bool COpBindings::Compile(List<Operator>* ops, UserInputs* usins, string* str, Bounds bnds) {

  this->op_ptr = nullptr;

  strloc cursor = bnds.strt;

  // find op identifier in keymap
  Bounds op_name = find_idname_straigt(str, IDNAME_BOUD, IDNAME_BOUD, Bounds(cursor, bnds.end));
  if (!valid(op_name)) {
    post_MSG(CMSGType::WARNING,
             "Compiling KeyMap: Op Identifier Not Found. Corrupted KeyMap file at " + bnds.strt);
    return false;
  }
  cursor = op_name.strt;

  // find op in prefs
  FOREACH_NODE(Operator, ops, op_node) {

    if (strs_match(&op_node->Data->idname, str, Bounds(0, op_node->Data->idname.length() - 1), op_name)) {
      this->op_ptr = op_node->Data;
      break;
    }
  }
  if (!op_ptr) {
    string warning_op;
    str_coppy(str, op_name, warning_op);
    post_MSG(CMSGType::WARNING, "Compiling KeyMap: Couldn't find spicified operator: " + warning_op);
    return false;
  }

  // find invoke
  Bounds invk = find_idname_straigt(str, CONDS_START, CONDS_END, Bounds(cursor, bnds.end));
  if (!valid(invk)) {
    post_MSG(CMSGType::WARNING,
             "Compiling KeyMap: Invoke Identifier Not Found. Corrupted KeyMap file at " + bnds.strt);
    return false;
  }
  // init invoke shortcut
  if (!(invoke.Compile(usins, str, invk))) {
    return false;
  }


  Bounds modalm = find_idname(str, MMAP_START, MMAP_END, bnds);
  if (!valid(invk)) {
    return false;
  }

  // Init modal items
  strloc mmp_cursor = modalm.strt;
  do {

    // find modal map item
    Bounds mmp_item = find_idname_straigt(str, MMPITEM_START, MMPITEM_END, Bounds(mmp_cursor, modalm.end));
    if (!valid(mmp_item)) {
      break;
    }

    mmp_cursor = mmp_item.strt;


    CShortcut* modal_shcut = new CShortcut();

    // find event identifier
    Bounds modal_event_name =
        find_idname_straigt(str, IDNAME_BOUD, IDNAME_BOUD, Bounds(mmp_cursor, mmp_item.end));
    if (!valid(modal_event_name)) {
      continue;
    }
    str_coppy(str, modal_event_name, modal_shcut->modal_event.idname);


    // find conditions
    Bounds conditions = find_idname_straigt(str, CONDS_START, CONDS_END, Bounds(mmp_cursor, mmp_item.end));
    if (!valid(conditions)) {
      continue;
    }

    // init conditions
    if (modal_shcut->Compile(usins, str, conditions)) {
      this->modal_keymap.add(modal_shcut);
    } else {
      delete modal_shcut;
    }

    mmp_cursor = mmp_item.end;

  } while (true);

  return true;
}

void CompiledKeyMap::Compile(List<Operator>* ops, UserInputs* usins, std::string* filipath) {

  // Loading File into string
  ifstream keymap_file;
  keymap_file.open(*filipath);
  if (!keymap_file) {
    post_MSG(CMSGType::ERRORtype, "Compiling KeyMap: Couldn't load the keymap");
  }

  stringstream strStream;
  strStream << keymap_file.rdbuf();
  string kmap = strStream.str();

  // Reading KeyMap
  strloc cursor = 0;
  do {

    Bounds bds = find_idname_straigt(&kmap, KB_START, KB_END, Bounds(cursor, kmap.length()));
    if (!valid(bds)) {
      break;
    }

    cursor = bds.strt;

    COpBindings* new_bdgs = new COpBindings();
    if (new_bdgs->Compile(ops, usins, &kmap, bds)) {
      this->op_bindings.add(new_bdgs);
    } else {
      delete new_bdgs;
    }

    cursor = bds.end;

  } while (true);

  keymap_file.close();
}
