#pragma once

#include "Vec2.h"
#include "LinkedList.h"
#include "Property.h"

enum class EditorType {
  VIEWPORT,
  HIERARCHYTREE,
  // ...
};

class Button {
 public:
  Button();
  ~Button();

  Rect<SCR_UINT> rect;
  struct Operator* op;

  void Poll(class Context* C);
  void Execute(class Context* C);

  void Draw(class Contex* C, class FBuff* Buff);

  Properties Props;

 private:
};

struct UIs {
  List<Button>* Buttons;
  Properties Props;
};

struct Region {
  Rect<SCR_UINT> rect;
  void (*Draw)(class Context* C, Region* region,
               RectPtr<SCR_UINT>* sa_rect, FBuff* Buff) = nullptr;
  class Editor* editor;
  List<UIs> UIs;
  Properties Props;
};

class Editor {
 public:
  EditorType Type;
  List<Region> Regions;
  Properties Props;

  Editor();
  ~Editor();
  void Draw(class Context* C, RectPtr<SCR_UINT>* rect, class FBuff* Buff);
};

class ScrArea {
 public:
  RectPtr<SCR_UINT> rect;
  Editor* editor;
  Properties Props;

  ScrArea(Editor* editor, RectPtr<SCR_UINT> rect);
  ~ScrArea();
};

void UI_Init(class Context* C);