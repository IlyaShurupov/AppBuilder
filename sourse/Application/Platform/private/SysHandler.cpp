
#include "Platform/SysHandler.h"

#include "UI/UInputs.h"
#include "UI/UInterface.h"

#ifdef WIN32
#include <libloaderapi.h>
#endif

char* getExecutablePath() {

  char path[50];
  for (int i = 0; i < 50; i++) {
    path[i] = 0;
  }

  #ifdef WIN32
  GetModuleFileNameA(nullptr, path, 50);
  #endif

  int endidx = 1;
  for (; endidx < 50; endidx++) {
    if (path[endidx] == 0) {
      break;
    }
  }

  char* trimed_path = new char[endidx];

  for (int i = 0; i < endidx; i++) {
    trimed_path[i] = path[i];
  }

  return trimed_path;
}

void UpdInputSate(Input& key, bool down, bool& IsEvent) {
  if ((int)key.state == (int)down) {
    return;
  }

  IsEvent = true;

  if (key.state == InputState::NONE) {
    key.state = InputState::PRESSED;
  } else if (key.state == InputState::HOLD) {
    key.state = InputState::RELEASED;
  } else {
    key.state = InputState(down);
  }
}

#ifdef WIN32
#include "Platform/private/windows/Win32Window.h"
#include <windows.h>

struct SyshWin32 {
  List<Win32Window> win32windows;
  bool consolehidden = false;
  Str icon;
  int scr_y = GetDeviceCaps(GetDC(NULL), VERTRES);
};

SysHandler::SysHandler() {
  PlatformDepended = NEW(SyshWin32)();
  IF(SUCCEEDED(CoInitialize(NULL)), );
  ConsoleToggle();
}

SysHandler::~SysHandler() {
  CoUninitialize();
}

void SysHandler::Inputs(UInputs& usin) {

  SyshWin32* dt = (SyshWin32*)PlatformDepended;

  usin.IsEvent = false;

  for (int i = 48; i < 58; i++) {
    UpdInputSate((&usin.K0)[i - 48], GetAsyncKeyState(i) & 0x8000, usin.IsEvent);
  }

  for (int i = 65; i < 90; i++) {
    UpdInputSate((&usin.A)[i - 65], GetAsyncKeyState(i) & 0x8000, usin.IsEvent);
  }

  UpdInputSate(usin.LMB, GetAsyncKeyState(VK_LBUTTON) & 0x8000, usin.IsEvent);
  UpdInputSate(usin.RMB, GetAsyncKeyState(VK_RBUTTON) & 0x8000, usin.IsEvent);
  UpdInputSate(usin.MMB, GetAsyncKeyState(VK_MBUTTON) & 0x8000, usin.IsEvent);

  UpdInputSate(usin.SPACE, GetAsyncKeyState(VK_SPACE) & 0x8000, usin.IsEvent);
  UpdInputSate(usin.ENTER, GetAsyncKeyState(VK_SEPARATOR) & 0x8000, usin.IsEvent);
  UpdInputSate(usin.DEL, GetAsyncKeyState(VK_DELETE) & 0x8000, usin.IsEvent);
  UpdInputSate(usin.EREASE, GetAsyncKeyState(VK_BACK) & 0x8000, usin.IsEvent);
  UpdInputSate(usin.ESCAPE, GetAsyncKeyState(VK_ESCAPE) & 0x8000, usin.IsEvent);
  UpdInputSate(usin.TAB, GetAsyncKeyState(VK_TAB) & 0x8000, usin.IsEvent);

  UpdInputSate(usin.WIN_KEY, GetAsyncKeyState(VK_LWIN) & 0x8000, usin.IsEvent);
  UpdInputSate(usin.SHIFT_L, GetAsyncKeyState(VK_LSHIFT) & 0x8000, usin.IsEvent);
  UpdInputSate(usin.SHIFT_R, GetAsyncKeyState(VK_RSHIFT) & 0x8000, usin.IsEvent);
  UpdInputSate(usin.CTR_L, GetAsyncKeyState(VK_LCONTROL) & 0x8000, usin.IsEvent);
  UpdInputSate(usin.CTR_R, GetAsyncKeyState(VK_RCONTROL) & 0x8000, usin.IsEvent);
  UpdInputSate(usin.ALT_L, GetAsyncKeyState(VK_MENU) & 0x8000, usin.IsEvent);
  UpdInputSate(usin.ALT_R, GetAsyncKeyState(VK_MENU) & 0x8000, usin.IsEvent);

  usin.PrevCursor = usin.Cursor;

  POINT cursor;
  GetCursorPos(&cursor);
  cursor.y = dt->scr_y - cursor.y;

  usin.Cursor.x = (SCR_INT)cursor.x;
  usin.Cursor.y = (SCR_INT)cursor.y;

  usin.Cdelta.x = usin.Cursor.x - usin.PrevCursor.x;
  usin.Cdelta.y = usin.Cursor.y - usin.PrevCursor.y;

  usin.IsEvent = usin.Cdelta.x || usin.Cdelta.y || usin.IsEvent;
}

void SysHandler::Output(UIItem* UIroot) {

  if (!UIroot) {
    return;
  }

  SyshWin32* dt = (SyshWin32*)PlatformDepended;

  FOREACH(&UIroot->hrchy.childs, UIItem, uiinode) {

    Win32Window* couple = nullptr;

    FOREACH(&dt->win32windows, Win32Window, winnode) {
      if (uiinode->hrchy.id == winnode->id) {
        couple = winnode.Data();
        break;
      }
    }

    if (!couple) {
      couple = NEW(Win32Window)(uiinode.Data());
      couple->SetIcon(dt->icon);
      couple->id = uiinode->hrchy.id;
      dt->win32windows.PushBack(couple);
    }

    couple->Draw(uiinode.Data());
    couple->editflag = 1;
  }

  FOREACH(&dt->win32windows, Win32Window, winnode) {
    if (!winnode->editflag) {
      dt->win32windows.Detach(winnode.node());
    }
  }

}

void SysHandler::ConsoleToggle() {
  SyshWin32* dt = (SyshWin32*)PlatformDepended;
  ShowWindow(::GetConsoleWindow(), dt->consolehidden ? SW_SHOW : SW_HIDE);
  dt->consolehidden = !dt->consolehidden;
}

void SysHandler::SetIcon(Str& stricon) {
  ((SyshWin32*)PlatformDepended)->icon = stricon;
}


bool SysHandler::Active() {
  SyshWin32* dt = (SyshWin32*)PlatformDepended;

  if (!dt->win32windows.Len()) {
    return true;
  }

  FOREACH(&dt->win32windows, Win32Window, winnode) {
    if (winnode->active()) {
      return true;
    }
  }
  return false;
}

#elif __APPLE__
#elif __linux__
#endif

