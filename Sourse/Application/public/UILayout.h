#pragma once

#include "LinkedList.h"
#include "Property.h"
#include "Vec2.h"
#include "FrameBuff.h"
#include "Operator.h"

enum struct UIInputState {
  NONE = 0,
  HOLD,
  PRESSED,
  RELEASED,
  HOVER,
};

enum struct UIType {
  NONE = 0,
  ROOT,
  AREA,
  REGION,
};

struct UIItem {

  Hierarchy<UIItem, List<UIItem>, 0> hierarchy;

  UIType type;

  void* CustomData = nullptr;

  // location & size in the parent frame of reference
  Rect<SCR_UINT> rect;
  
  void (*ProcEvent)(UIItem *This, List<OpThread>* op_threads, struct UserInputs *user_inputs) = nullptr;
  void (*Draw)(UIItem *This) = nullptr;

  UIItem(UIType UIType) {
    type = UIType;
  }
};

UIItem* UIroot_Init(List<Operator>* operators);