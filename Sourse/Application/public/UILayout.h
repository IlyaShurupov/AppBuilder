#pragma once

#include "LinkedList.h"
#include "Property.h"
#include "Vec2.h"
#include "FrameBuff.h"

enum struct UIInputState {
  NONE = 0,
  HOLD,
  PRESSED,
  RELEASED,
  HOVER,
};

struct UIItem {

  // All ui items wich this item is build of
  Hierarchy<UIItem, List<UIItem>, 0> hierarchy;

  // location & size in the parent frame of reference
  Rect<SCR_UINT> rect;
  
  // Own buffer to daraw
  FBuff<RGBA_32> *buffer;

  // current input state of ui item
  UIInputState input_state;

  void (*ProcEvent)(UIItem *This, struct UserInputs *user_inputs);
  void (*Draw)(UIItem *This);
};

void UI_Init(struct Seance *C);