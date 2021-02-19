#pragma once

#include "Color/FrameBuff.h"
#include <Windows.h>
#include "Strings.h"

struct Win32Window {

  Win32Window(struct UIItem* uii);
  ~Win32Window();

  void Draw(struct UIItem* uii);

  void ProcSysEvents();

  bool active();
  void SetIcon(struct Str& stricon);

  char editflag;
  Str id;

 private:
  
  int scr_y;
  void* hWindowIcon;
  void* hWindowIconBig;
  void* hdcMem;
  void* m_hwnd;
  void* m_pDirect2dFactory;
  void* msg;
  Rect<float> prevrec;
  static __int64 __stdcall win_proc(HWND hwnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);
};