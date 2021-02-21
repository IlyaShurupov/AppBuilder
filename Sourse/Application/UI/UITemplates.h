#pragma once

#include "Containers/List.h"
#include "UI/UInterface.h"

struct DataBlock;
class Operator;

class UIIButton : UIItem {

  Operator* target = nullptr;
  OpThread* thread = nullptr;

  bool onpress = false;
  COLOR col_in;
  COLOR col_out;
  COLOR col_hold;

  OpArg pressed;
  OpArg hold;
  OpArg released;

  bool drawhold = false;

public:
  void DrawBody(UIItem* This, UIItem* draw_to);
  void ProcBody(UIItem* This, Seance* C, vec2<SCR_INT>& loc_cursor);
  UIIButton(Operators* ops, DataBlock* paramsdb, DataBlock* uiidb);
  ~UIIButton();
};

class UIIGroup : UIItem {

  COLOR Framein;
  COLOR Frameout;
  COLOR Fillin;
  COLOR Fillout;

  bool frame = true;
  bool fill = false;
  int thickin;
  int thickout;

public:
  UIIGroup(Operators* ops, DataBlock* paramsdb, DataBlock* uiidb);
  ~UIIGroup();
  void ProcBody(UIItem* This, Seance* C, vec2<SCR_INT>& loc_cursor);
  void DrawBody(UIItem* This, UIItem* draw_to);
};