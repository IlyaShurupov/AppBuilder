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

  Rect<short> rect;
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
  Rect<short> rect;
  void (*Draw)(class Context* C, Region* region, FBuff* Buff) = nullptr;

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
  void Draw(class Context* C, RectPtr<short>* rect, class FBuff* Buff);
};

class ScrArea {
 public:
  RectPtr<short> Rect;
  Editor* Editor;
  Properties Props;

  ScrArea();
  ~ScrArea();
};