
#include "Platform/SysHandler.h"

#include "UI/UInputs.h"
#include "UI/Window.h"
#include "UI/UInterface.h"
#include "Platform/private/Win32Window.h"

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

#include <windows.h>

struct SyshWin32 {
  List<Win32Window> win32windows;
  bool consolehidden = false;
  Str icon;
  int scr_y = GetDeviceCaps(GetDC(NULL), VERTRES);
};

SysHandler::SysHandler() {
  PlatformDepended = NEW_DBG(SyshWin32) SyshWin32();
  IF(SUCCEEDED(CoInitialize(NULL)));
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

  usin.Cursor.x = (SCR_UINT)cursor.x;
  usin.Cursor.y = (SCR_UINT)cursor.y;

  usin.Cdelta.x = usin.Cursor.x - usin.PrevCursor.x;
  usin.Cdelta.y = usin.Cursor.y - usin.PrevCursor.y;

  usin.IsEvent = usin.Cdelta.x || usin.Cdelta.y || usin.IsEvent;
}

void SysHandler::Output(UIItem* UIroot) {

  SyshWin32* dt = (SyshWin32*)PlatformDepended;

  FOREACH(&UIroot->hrchy.childs, UIItem, uiinode) {

    Win32Window* couple = nullptr;

    FOREACH(&dt->win32windows, Win32Window, winnode) {
      if (uiinode->Data->hrchy.id == winnode->Data->id) {
        couple = winnode->Data;
        break;
      }
    }

    if (!couple) {
      couple = NEW_DBG(Win32Window) Win32Window(uiinode->Data);
      couple->SetIcon(dt->icon);
      couple->id = uiinode->Data->hrchy.id;
      dt->win32windows.add(couple);
    }

    couple->Draw(uiinode->Data);
    couple->editflag = 1;
  }

  FOREACH(&dt->win32windows, Win32Window, winnode) {
    if (!winnode->Data->editflag) {
      dt->win32windows.del(winnode);
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

#elif __APPLE__
#elif __linux__
#endif

