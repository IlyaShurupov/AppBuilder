
#include "public/Window.h"
#include "public/Operator.h"
#include "public/Win32API.h"
#include "public/UInputsMap.h"
#include "public/UInterface.h"

//#include "public/Print.h"

Window::Window(Str* configfolder, List<Operator>* operators) {
  user_inputs = NEW_DBG(UInputs) UInputs();

  Str ui_path;
  ui_path = *configfolder;
  ui_path += Str("UInterface\\Default.yaml");
  DataBlock* uidb = Read_Yaml(&ui_path);
  UIroot = UICompile(operators, uidb, this);

  Str km_path;
  km_path = *configfolder;
  km_path += Str("UInputsMap\\Default.yaml");
  DataBlock* kmdb = Read_Yaml(&km_path);
  keymap = NEW_DBG(KeyMap) KeyMap();
  keymap->Compile(kmdb, operators, user_inputs, UIroot);

  // init sys handler
  Rect<SCR_UINT> rect(UIroot->rect);
  SysH = NEW_DBG(SystemHandler) SystemHandler(rect);

  // Set icon
  Str icon_path; icon_path = *configfolder;
  icon_path += Str("icon.ico");
  SysH->SetIcon(icon_path);

  SysH->getScreenSize(scr_size);

  // draw initialized window
  Draw();

  SysH->setRect(rect, scr_size.y);
  SysH->ShowInitializedWindow();
  SendBuffToSystem();
}

Window::~Window() {
  DELETE_DBG(UIItem, UIroot);
  DELETE_DBG(UInputs, user_inputs);
  DELETE_DBG(SystemHandler, SysH);
}

void Window::OnWrite() {}

void Window::OnRead() {}

void Window::Draw() {
  UIroot->Draw(nullptr);
}

void Window::ProcessEvents(List<OpThread>* op_threads, Seance* C) {
  SysH->getUserInputs(user_inputs, scr_size.y);
  if (this->IsActive() && user_inputs->IsEvent) {
    keymap->evaluate(op_threads);
  }
  vec2<SCR_UINT> pos = vec2<SCR_UINT>((SCR_UINT)UIroot->rect.pos.x, (SCR_UINT)UIroot->rect.pos.y);
  UIroot->ProcEvent(op_threads, user_inputs, user_inputs->PrevCursor + pos, C);
}

void Window::SendBuffToSystem() {
  SysH->SysOutput(UIroot->buff);
}

bool Window::IsActive() {
  return SysH->active();
}

void Window::ToggleConsole() {
  SysH->consoletoggle();
}

void Window::getRect(Rect<SCR_UINT>& rect) {
  rect = Rect<SCR_UINT>(UIroot->rect);
}

void Window::setRect(Rect<SCR_UINT>& newrect) {

  // SysH->getRect(rect, scr_size.y);

  // clamp new rect first
  if (newrect.size.x < UIroot->minsize.x) {
    if (newrect.pos.x > UIroot->rect.pos.x) {
      // left -> right
      newrect.pos.x = (SCR_UINT)UIroot->rect.size_vec_w().x - UIroot->minsize.x;
      newrect.size.x = UIroot->minsize.x;

    } else {
      // left <- right
      newrect.size.x = UIroot->minsize.x;
    }
  }

  if (newrect.size.y < UIroot->minsize.y) {
    if (newrect.pos.y > UIroot->rect.pos.y) {
      // top <- bottom
      newrect.pos.y = (SCR_UINT)UIroot->rect.size_vec_w().y - UIroot->minsize.y;
      newrect.size.y = UIroot->minsize.y;

    } else {
      // top -> bottom
      newrect.size.y = UIroot->minsize.y;
    }
  }

  // update cursor pos
  user_inputs->Cursor.x -= newrect.pos.x - (SCR_UINT)UIroot->rect.pos.x;
  user_inputs->Cursor.y -= newrect.pos.y - (SCR_UINT)UIroot->rect.pos.y;

  Rect<float> flt_rec;
  flt_rec.size.assign((float)newrect.size.x, (float)newrect.size.y);
  flt_rec.pos.assign((float)newrect.pos.x, (float)newrect.pos.y);

  UIroot->Resize(flt_rec);

  newrect.pos.assign((SCR_UINT)UIroot->rect.pos.x, (SCR_UINT)UIroot->rect.pos.y);
  newrect.size.assign((SCR_UINT)UIroot->rect.size.x, (SCR_UINT)UIroot->rect.size.y);
  SysH->setRect(newrect, scr_size.y);
  // SendBuffToSystem();
}
