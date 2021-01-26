#pragma once

#include "LinkedList.h"
#include "Vec2.h"
#include "FrameBuff.h"
#include "Operator.h"

enum struct UICursorState {
  NONE = 0,
  ENTERED,
  INSIDE,
  LEAVED,
};

struct UIItem {

  Hierarchy<UIItem, List<UIItem>, 0> hierarchy;

  UICursorState ev_state;
  Rect<SCR_UINT> rect;
  vec2<SCR_UINT> minsize;

  bool visible;
  bool redraw = true;
  FBuff<RGBA_32> *buff = nullptr;

  void upd_ev_state(vec2<SCR_UINT>& cursor, struct UserInputs* user_inputs);
  void (*ProcEvent)(UIItem *This, List<OpThread>* op_threads, struct UserInputs *user_inputs, vec2<SCR_UINT> &loc_cursor, Seance* C);
  void (*Draw)(UIItem *This, UIItem* project_to) = nullptr;
  void* CustomData = nullptr;

  UIItem(vec2<SCR_UINT>* size) {

    ev_state = UICursorState::NONE;
    ProcEvent = nullptr;

    if (this->visible = (bool)size) {
      buff = DBG_NEW FBuff<RGBA_32>();
      buff->resize(size->x, size->y);
    }
  }
};

UIItem* UI_compile(List<Operator>* operators, std::string* ui_path, struct Window* parent);