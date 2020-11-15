
#include "public/Window.h"

#include "public/Operator.h"
#include "public/Win32API.h"

void Window::Draw() {
  printf("\n %i ", user_inputs.Cdelta.x);
  printf(" %i ", user_inputs.Cdelta.y);
}

Window::Window(std::string* configfolder, List<Operator>* operators) {
  HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
  SystemHandler* SysHdl = new SystemHandler();

  if (!SUCCEEDED(CoInitialize(NULL))) {
    printf("ERROR: imma bout to crash\n");
  }

  if (!SUCCEEDED(SysHdl->Initialize(vec2<SCR_UINT>(800, 800)))){
    printf("ERROR: system handler is out of his mind\n");
  }
  
  this->SysH = SysHdl;

  std::string keymap_path = *configfolder + "KeyMaps\\Default.txt";
  std::string icon_path = *configfolder + "icon.ico";
  SysH->SetIcon(icon_path);

  minsize.y = minsize.x = 30;
  buff.Resize(800, 800);
  SysH->getScreenSize(scr_size);
  compiled_key_map.Compile(operators, &user_inputs, &keymap_path);
}

Window::~Window() { CoUninitialize(); }

void Window::OnWrite() {}

void Window::OnRead() {}

void Window::ProcessEvents(List<OpThread>* op_threads) {
  SysH->getUserInputs(&user_inputs, scr_size.y);
  if (this->IsActive()) {
    compiled_key_map.ProcEvents(op_threads);
  }
}

void Window::SendBuffToSystem() {
  SysH->SysOutput(&buff);
}

bool Window::IsActive() {
  return SysH->active();
}

void Window::Destroy() {
  SysH->destroy();
}

void Window::ToggleConsole() {

  SysH->consoletoggle();
}

void Window::getRect(Rect<SCR_UINT>& rect) {
  SysH->getRect(rect, scr_size.y);
}

void Window::setRect(Rect<SCR_UINT>& newrect) {

  SysH->getRect(rect, scr_size.y);

  // clamp new rect first
  if (newrect.size.x < minsize.x) {
    if (newrect.pos.x > rect.pos.x) {
      // left -> right
      newrect.pos.x = rect.size_vec_w().x - minsize.x;

    } else {
      // left <- right
      newrect.size.x = minsize.x;
    }
  }

  if (newrect.size.y < minsize.y) {
    if (newrect.pos.y > rect.pos.y) {
      // top <- bottom
      newrect.pos.y = rect.size_vec_w().y - minsize.y;

    } else {
      // top -> bottom
      newrect.size.y = minsize.y;
    }
  }
  
  // update cursor pos
  user_inputs.Cursor.x -= newrect.pos.x - rect.pos.x;
  user_inputs.Cursor.y -= newrect.pos.y - rect.pos.y;

  this->rect = newrect;

  // upd buffer
  this->buff.Resize(rect.size.x, rect.size.y);

  SysH->setRect(rect, scr_size.y);
}
