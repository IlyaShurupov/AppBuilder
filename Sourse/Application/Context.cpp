#pragma once
#include "public/Context.h"

Screen::Screen() {
  for (int i = 0; i < EventState.num; i++) {
    (&EventState.A)[i] = EVENT_NONE;
  }

  Fbuff = nullptr;
}

Screen::~Screen() {}

void Screen::UpdEventState() {}

bool Screen::IsEvent() { return true; }

void Screen::Draw(class Context* C) {
  FOREACH_NODE(ScrArea, (&ScrAreas), edt_node) { 
    Editor* edt = edt_node->Data->Editor;
    edt->Draw(C, &edt_node->Data->Rect, Fbuff);
  }
}

Context::Context() {}

Context::~Context() {}

Screen* Context::getActiveScreen() { return ActiveWin; }
