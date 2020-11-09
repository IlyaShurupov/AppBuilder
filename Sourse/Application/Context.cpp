#pragma once
#include "public/Context.h"

AppEvent::AppEvent(void (*Update)(AppEvent* ThisEvent)) {
  for (int i = 0; i < KeyNum; i++) {
    (&A)[i] = KeyState::EVENT_NONE;
  }
}

AppEvent::~AppEvent() {}

bool AppEvent::IsEvent() { return true; }

AppWindow::AppWindow(void (*SysOutput)(), FBuff* SysBuff) {
  fbuff = SysBuff;
  this->SysOutput = SysOutput;
}

AppWindow::~AppWindow() {}

void AppWindow::Draw(class Context* C) {
  // FOREACH_NODE(ScrArea, (&ScrAreas), edt_node) {
  // Editor* edt = edt_node->Data->Editor;
  // edt->Draw(C, &edt_node->Data->Rect, Fbuff);
  //}
}

Context::Context(void (*SysOutput)(), void (*Inputs)(AppEvent* Event),
                 FBuff* SysBuff)
    : window(SysOutput, SysBuff), event(Inputs) {

  /*
  * void (*Inputs)(Event* Event)
  Screen* ActScr = new Screen;
  C->screen = ActScr;
  C->Screens.add(ActScr);
  ActScr->Event.Update = Inputs;
  ActScr.Fbuff = SysHdl.getFBuff();
  */
}

Context::~Context() {}