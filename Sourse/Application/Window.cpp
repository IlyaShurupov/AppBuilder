
#include "public/Window.h"

#include "public/Operator.h"
#include "public/Win32API.h"

void Window::Draw() {
  UIroot->Draw(UIroot, NULL);
}

Window::Window(std::string* configfolder, List<Operator>* operators) {

  // compile kmap
  std::string keymap_path = *configfolder + "KeyMaps\\Default.txt";
  compiled_key_map.Compile(operators, &user_inputs, &keymap_path);

  std::string ui_path = *configfolder + "UIs\\Default.txt";
  UIroot = UI_compile(operators, &ui_path, this);

  
  // init sys handler
  SysH = DBG_NEW SystemHandler();

  HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
  if (!SUCCEEDED(CoInitialize(NULL))) {
    printf("ERROR: im about to crash\n");
  }
  if (!SUCCEEDED(SysH->Initialize(UIroot->rect))) {
    printf("ERROR: system handler is out of his mind\n");
  }

  // Set icon
  std::string icon_path = *configfolder + "icon.ico";
  SysH->SetIcon(icon_path);

  SysH->getScreenSize(scr_size);

  // draw initialized window
  Draw();

  SysH->setRect(UIroot->rect, scr_size.y);
  SysH->ShowInitializedWindow();
  SendBuffToSystem();

}

Window::~Window() {
  //compiled_key_map.op_bindings.free();
  delete SysH;
  CoUninitialize();
}

void Window::OnWrite() {}

void Window::OnRead() {}

void Window::ProcessEvents(List<OpThread>* op_threads) {
  SysH->getUserInputs(&user_inputs, scr_size.y);
  if (this->IsActive() && user_inputs.IsEvent) {
    compiled_key_map.ProcEvents(op_threads);
  }
  UIroot->ProcEvent(UIroot, op_threads, &user_inputs, user_inputs.Cursor);
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
  rect = UIroot->rect;
}

void Window::setRect(Rect<SCR_UINT>& newrect) {

  //SysH->getRect(rect, scr_size.y);

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

  UIroot->rect = newrect;
  UIroot->buff->resize(newrect.size.x, newrect.size.y);

  SysH->setRect(newrect, scr_size.y);
  // SendBuffToSystem();
}
