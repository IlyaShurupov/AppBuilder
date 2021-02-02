#pragma once

#include "FrameBuff.h"
#include "Operator.h"

enum struct UIstate {
  NONE = 0,
  ENTERED,
  INSIDE,
  LEAVED,
};

enum struct UIResizeType {
  NONE = 0,
  FOLLOW,
  FOLLOW_X,
  FOLLOW_Y,
};

enum struct UIAttachCorner {
  TOPLEFT = 0,
  BOTTOMLEFT,
  BOTTOMRIGGHT,
  TOPRIGHT,
};

struct Wrap {
  struct UIItem* top = nullptr;
  struct UIItem* bot = nullptr;
  struct UIItem* lef = nullptr;
  struct UIItem* rig = nullptr;
};

struct UIItem {

  UIItem(vec2<SCR_UINT>* size);
  ~UIItem();

  Hierarchy<UIItem, List<UIItem>, 0> hierarchy;
  Str idname;

  // Event info
  UIstate state;
  bool redraw = true;

  // Edit info
  bool hide = false;
  Rect<float> rect;
  Rect<float> prev_rect;
  vec2<SCR_UINT> minsize;
  UIResizeType rs_type;
  UIAttachCorner crnr;
  Wrap wrap;
  char flag;

  // Draw info
  bool ownbuff = true;
  FBuff<RGBA_32> *buff = nullptr;

  // User difined callbacks wrapers
  void Resize(vec2<float>& rescale);
  void ProcEvent(List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT>& loc_cursor, Seance* C);
  void Draw(UIItem* project_to);

  // User defined callbacks
  void (*ProcBody)(UIItem *This, List<OpThread>* op_threads, struct UserInputs *user_inputs, vec2<SCR_UINT> &loc_cursor, Seance* C);
  void (*DrawBody)(UIItem* This, UIItem* project_to);
  void* CustomData = nullptr;

  void update_neighbors(bool recursive);
};

UIItem* UI_compile(List<Operator>* operators, Str* ui_path, struct Window* parent);