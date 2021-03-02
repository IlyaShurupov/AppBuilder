#pragma once

#include "BitMap/BitMap.h"
#include "Object/Hierarchy.h"

struct Operators;
struct DataBlock;

enum struct UIIstate {
  NONE = 0,
  ENTERED,
  INSIDE,
  LEAVED,
};

class UIItem {
 public:

  Hrchy<UIItem> hrchy;

  // Draw info
  Rect<float> rect;
  BitMap<RGBA_32>* buff = nullptr;

  bool ownbuff = true;
  bool recursive_trunsform = false;
  vec2<bool> rigid;

  // Event info
  UIIstate state;
  bool hide = false;
  bool redraw = true;

  // Edit info
  Rect<float> prev_rect;
  char flag = 0;

  UIItem(struct DataBlock* UIdb);
  ~UIItem();

  // User difined callbacks wrapers
  void Resize(Rect<float>& newrect);
  void ProcEvent(struct Seance* C, vec2<SCR_INT>& loc_cursor);
  void Draw(UIItem* project_to);

  // User defined callbacks
  virtual void ProcBody(UIItem* This, Seance* C, vec2<SCR_INT>& loc_cursor) {}
  virtual void DrawBody(UIItem* This, UIItem* draw_to) {}
  virtual void ResizeBody(Rect<float>& out, bool dir) {}
  virtual bool Valid(bool dir) { return true; }

  UIItem* find(struct Str* string);
  UIItem* active_lower();
  void MoveChilds(vec2<float>& delta);
  void move(vec2<float> pos);
  void PosInParent(UIItem* inframe, vec2<float>& out);
  void save_config();
  void resize_discard(bool dir);
  void clear_flags();
  void update_buff(bool recursive);

 private:
  bool resize_dir(float rescale, bool dir, bool& root);
};

UIItem* UICompile(Operators* ops, DataBlock* db, UIItem* (*UIIFromStr)(Str* id, Operators* ops, DataBlock* paramsdb, DataBlock* uiidb));