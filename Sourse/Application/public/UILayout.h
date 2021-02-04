#pragma once

#include "FrameBuff.h"
#include "Operator.h"

enum struct UIstate {
  NONE = 0,
  ENTERED,
  INSIDE,
  LEAVED,
};

struct Wrap {
  struct UIItem* rig = nullptr;
  struct UIItem* top = nullptr;
  struct UIItem* lef = nullptr;
  struct UIItem* bot = nullptr;
};

struct UIItem {

  UIItem(vec2<SCR_UINT>* size);
  ~UIItem();

  Hierarchy<UIItem, List<UIItem>, 0> hrchy;
  Str idname;

  // Event info
  UIstate state;
  bool redraw = true;

  // Edit info
  bool hide = false;
  Rect<float> rect;
  Rect<float> prev_rect;
  vec2<float> minsize;
  vec2<bool> rigid;
  vec2<bool> inv_pos;
  Wrap wrap;
  char flag = 0;

  // Draw info
  bool ownbuff = true;
  FBuff<RGBA_32>* buff = nullptr;

  // User difined callbacks wrapers
  void Resize(Rect<float>& newrect);
  void ProcEvent(List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT>& loc_cursor, Seance* C);
  void Draw(UIItem* project_to);

  // Save UI of window
  void OnWrite();

  // Restore saved UI
  void OnRead();

  // User defined callbacks
  void (*ProcBody)(UIItem* This, List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT>& loc_cursor, Seance* C);
  void (*DrawBody)(UIItem* This, UIItem* project_to);
  void* CustomData = nullptr;

 private:
  void resize_discard(bool dir);
  bool resize_dir(float rescale, bool dir);
  void update_neighbors(bool recursive);
  void update_buff(bool recursive);
  void ResizeBody(Rect<float>& out, bool dir);
  void clear_flags();
  void save_config();
  bool valid_resize(Rect<float>& newrec, bool dir);
};

UIItem* UI_compile(List<Operator>* operators, Str* ui_path, struct Window* prnt);