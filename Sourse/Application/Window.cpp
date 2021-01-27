
#include "public/Window.h"

#include "public/Operator.h"
#include "public/Win32API.h"

Window::Window(Str* configfolder, List<Operator>* operators) {

  // compile kmap
  Str keymap_path;
  keymap_path = *configfolder;
  keymap_path += Str("KeyMaps\\Default.txt");
  compiled_key_map.Compile(operators, &user_inputs, &keymap_path);

  Str ui_path;
  ui_path = *configfolder;
  ui_path += Str("UIs\\Default.txt");
  UIroot = UI_compile(operators, &ui_path, this);


  // init sys handler
  SysH = DBG_NEW SystemHandler();

  HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
  if (!SUCCEEDED(CoInitialize(NULL))) {
    printf("ERROR: im about to crash\n");
  }
  Rect<SCR_UINT> rect =
      Rect<SCR_UINT>(UIroot->rect.pos.x, UIroot->rect.pos.y, UIroot->rect.size.x, UIroot->rect.size.y);
  if (!SUCCEEDED(SysH->Initialize(rect))) {
    printf("ERROR: system handler is out of his mind\n");
  }

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
  // compiled_key_map.op_bindings.free();
  delete SysH;
  delete UIroot;
  CoUninitialize();
}

void Window::OnWrite() {}

void Window::OnRead() {}

void Window::Draw() {
  UIroot->Draw(NULL);
}

void Window::ProcessEvents(List<OpThread>* op_threads, Seance* C) {
  SysH->getUserInputs(&user_inputs, scr_size.y);
  if (this->IsActive() && user_inputs.IsEvent) {
    compiled_key_map.ProcEvents(op_threads);
  }
  vec2<SCR_UINT> pos = vec2<SCR_UINT>(UIroot->rect.pos.x, UIroot->rect.pos.y);
  UIroot->ProcEvent(op_threads, &user_inputs, user_inputs.Cursor + pos, C);
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
  rect = Rect<SCR_UINT>(UIroot->rect.pos.x, UIroot->rect.pos.y, UIroot->rect.size.x, UIroot->rect.size.y);
}

void Window::setRect(Rect<SCR_UINT>& newrect) {

  // SysH->getRect(rect, scr_size.y);

  // clamp new rect first
  if (newrect.size.x < UIroot->minsize.x) {
    if (newrect.pos.x > UIroot->rect.pos.x) {
      // left -> right
      newrect.pos.x = UIroot->rect.size_vec_w().x - UIroot->minsize.x;
      newrect.size.x = UIroot->minsize.x;

    } else {
      // left <- right
      newrect.size.x = UIroot->minsize.x;
    }
  }

  if (newrect.size.y < UIroot->minsize.y) {
    if (newrect.pos.y > UIroot->rect.pos.y) {
      // top <- bottom
      newrect.pos.y = UIroot->rect.size_vec_w().y - UIroot->minsize.y;
      newrect.size.y = UIroot->minsize.y;

    } else {
      // top -> bottom
      newrect.size.y = UIroot->minsize.y;
    }
  }

  // update cursor pos
  user_inputs.Cursor.x -= newrect.pos.x - UIroot->rect.pos.x;
  user_inputs.Cursor.y -= newrect.pos.y - UIroot->rect.pos.y;

  UIroot->Resize(UIroot, vec2<float>((float)newrect.size.x / UIroot->rect.size.x,
                                     (float)newrect.size.y / UIroot->rect.size.y));
  UIroot->rect.pos.assign(newrect.pos.x, newrect.pos.y);

  SysH->setRect(newrect, scr_size.y);
  // SendBuffToSystem();
}
