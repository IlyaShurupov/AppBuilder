#include "public/UI.h"

#include "public/Context.h"

Button::Button() {}

Button::~Button() {}

void Button::Poll(Context* C) {}

void Button::Execute(Context* C) {}

void Button::Draw(Contex* C, FBuff* Buff) {}

Editor::Editor() { this->Type = EditorType::VIEWPORT; }

Editor::~Editor() {}

void Editor::Draw(Context* C, RectPtr<SCR_UINT>* rect, FBuff* Buff) {
  FOREACH_NODE(Region, (&Regions), reg_node) {
    Region* reg = reg_node->Data;
    reg->Draw(C, reg, Buff);
  }
}

ScrArea::ScrArea(Editor* editor, RectPtr<SCR_UINT> rect) {
  this->editor = editor;

  this->rect.v0 = rect.v0;
  this->rect.v1 = rect.v1;
  this->rect.v2 = rect.v2;
  this->rect.v3 = rect.v3;
}

ScrArea::~ScrArea() {}

void RegionViewport(class Context* C, Region* region, FBuff* Buff) {

  region->rect.v0.assign(50, 50);
  region->rect.v1.assign(50, 500 + 50);
  region->rect.v2.assign(50 + 500, 50 + 500);
  region->rect.v3.assign(50 + 500, 50);

  FBuff buff2;
  //Buff->cast(buff2, &region->rect);
  Camera* cam = C->RndrSets.getCamera()->GetCameraComponent();

  /*
  cam->Height.val = Buff->height;
  cam->Width.val = Buff->width;

  RayCast::RenderToBuff(&C->RndrSets, Buff);
  */

  cam->Height.val = buff2.height;
  cam->Width.val = buff2.width;

  RayCast::RenderToBuff(&C->RndrSets, &buff2);
}

void UI_Init(Context* C) {
  Editor* editor = new Editor;
  C->editors.add(editor);

  Region* reg = new Region;
  reg->editor = editor;
  reg->Draw = RegionViewport;
  editor->Regions.add(reg);
}
