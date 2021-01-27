#pragma once

#include "LinkedList.h"
#include "Vec2.h"
#include "FrameBuff.h"
#include "Operator.h"

enum struct UIstate {
  NONE = 0,
  ENTERED,
  INSIDE,
  LEAVED,
};

struct UIItem {

  Hierarchy<UIItem, List<UIItem>, 0> hierarchy;

  Str idname;
  UIstate state;

  Rect<float> rect;
  vec2<SCR_UINT> minsize;

  bool ownbuff;
  FBuff<RGBA_32> *buff = nullptr;

  bool redraw = true;
  void* CustomData = nullptr;

  void(*Resize)(UIItem* This, vec2<float>& rescale);
  void (*ProcBody)(UIItem *This, List<OpThread>* op_threads, struct UserInputs *user_inputs, vec2<SCR_UINT> &loc_cursor, Seance* C);
  void (*DrawBody)(UIItem* This, UIItem* project_to);

  void ProcEvent(List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT>& loc_cursor, Seance* C);
  void Draw(UIItem* project_to);

  UIItem(vec2<SCR_UINT>* size);
  ~UIItem();
};

UIItem* UI_compile(List<Operator>* operators, Str* ui_path, struct Window* parent);