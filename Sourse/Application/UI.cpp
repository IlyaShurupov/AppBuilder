#include "public/UI.h"
#include "public/Context.h"

Button::Button() {}

Button::~Button() {}

void Button::Poll(Context* C) {}

void Button::Execute(Context* C) {}

void Button::Draw(Contex* C, FBuff* Buff) {}

Editor::Editor() { this->Type = EditorType::VIEWPORT; }

Editor::~Editor() {}

void Editor::Draw(Context* C, RectPtr<short>* rect, FBuff* Buff) {
  FOREACH_NODE(Region, (&Regions), reg_node) {
    Region* reg = reg_node->Data;
    reg->Draw(C, reg, Buff);
  }
}

ScrArea::ScrArea(Editor* editor, RectPtr<short> rect) {
  this->editor = editor;

  this->rect.v0 = rect.v0;
  this->rect.v1 = rect.v1;
  this->rect.v2 = rect.v2;
  this->rect.v3 = rect.v3;
}

ScrArea::~ScrArea() {}


void RegionViewport(class Context* C, Region* region, FBuff* Buff) { 
  RayCast::RenderToBuff(&C->RndrSets, Buff);
}

void UI_Init(Context* C) {
  Editor * editor = new Editor;
  C->editors.add(editor);

  Region* reg = new Region;
  reg->Draw = RegionViewport;
  editor->Regions.add(reg);
}
