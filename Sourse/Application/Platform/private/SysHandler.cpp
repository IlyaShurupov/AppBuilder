
#include "Platform/SysHandler.h"

#include "UI/UInputs.h"
#include "UI/Window.h"


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

SysHandler::SysHandler() {
  CoInitialize(NULL);
  scrsize.y = GetDeviceCaps(GetDC(NULL), VERTRES);
  scrsize.x = GetDeviceCaps(GetDC(NULL), HORZRES);
}

SysHandler::~SysHandler() {
  CoUninitialize();
}

void SysHandler::UserInputs(UInputs& usin) {

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
  cursor.y = scrsize.y - cursor.y;

  usin.Cursor.x = (SCR_UINT)cursor.x;
  usin.Cursor.y = (SCR_UINT)cursor.y;

  usin.Cdelta.x = usin.Cursor.x - usin.PrevCursor.x;
  usin.Cdelta.y = usin.Cursor.y - usin.PrevCursor.y;

  usin.IsEvent = usin.Cdelta.x || usin.Cdelta.y || usin.IsEvent;
}

void SysHandler::Output(List<Window>& windows) {}

void SysHandler::ConsoleToggle() {}

void SysHandler::SetIcon(Str& stricon) {}

#elif __APPLE__
#elif __linux__
#endif

