
#include "public/Window.h"

#include "public/Operator.h"
#include "public/Win32API.h"

void Window::Draw() {
  FBFF_COLOR color = int32_t(0xff1b1b1f);
  if (timer.remain() > 0) {
    RGBA_SET((&color), unsigned char(255 * timer.ease_in()), 3);
    buff.usealpha = true;

    //rect.size.y += 20;
    rect.pos.y += -10;
    //rect.size.y += 8;
    //rect.pos.y -= 0.01 * timer.ease_out();
    //setRect(rect);
  } else {
    //buff.usealpha = false;
  }

  buff.clear(&color);
}

Window::Window(std::string* configfolder, List<Operator>* operators) {

  rect.size.assign(800, 500);
  rect.pos.assign(600, 250);
  minsize.y = 60;
  minsize.x = 100;

  // init sys handler
  SysH = DBG_NEW SystemHandler();

  HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
  if (!SUCCEEDED(CoInitialize(NULL))) {
    printf("ERROR: im about to crash\n");
  }
  if (!SUCCEEDED(SysH->Initialize(rect))) {
    printf("ERROR: system handler is out of his mind\n");
  }

  // Set icon
  std::string icon_path = *configfolder + "icon.ico";
  SysH->SetIcon(icon_path);

  // compile kmap
  std::string keymap_path = *configfolder + "KeyMaps\\Default.txt";
  compiled_key_map.Compile(operators, &user_inputs, &keymap_path);

  timer.duration = 400;
  timer.reset();
  //timer.start -= 500;

  SysH->getScreenSize(scr_size);

  // draw initialized window
  buff.resize(rect.size.x, rect.size.y);
  Draw();

  SysH->setRect(rect, scr_size.y);
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
}

void Window::SendBuffToSystem() {
  SysH->SysOutput(&buff);
}

bool Window::IsActive() {
  return SysH->active();
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
      newrect.size.x = minsize.x;

    } else {
      // left <- right
      newrect.size.x = minsize.x;
    }
  }

  if (newrect.size.y < minsize.y) {
    if (newrect.pos.y > rect.pos.y) {
      // top <- bottom
      newrect.pos.y = rect.size_vec_w().y - minsize.y;
      newrect.size.y = minsize.y;

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
  this->buff.resize(rect.size.x, rect.size.y);

  SysH->setRect(rect, scr_size.y);
  // SendBuffToSystem();
}
