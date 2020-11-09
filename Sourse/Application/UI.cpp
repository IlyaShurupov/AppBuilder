#include "public/UI.h"

#include "public/Context.h"

Button::Button() {}

Button::~Button() {}

void Button::Poll(Context* C) {}

void Button::Execute(Context* C) {}

void Button::Draw(Contex* C, FBuff* Buff) {}

Editor::Editor() { this->Type = EditorType::VIEWPORT; }

Editor::~Editor() {}

void Editor::Draw(Context* C, RectPtr<short>* rect, FBuff* Buff) {}

ScrArea::ScrArea() {
  Editor = nullptr;
}

ScrArea::~ScrArea() {}
