#pragma once

#include "BitMap/BitMap.h"
#include <windows.h>
#include "Strings/Strings.h"

struct Win32Window {

  Win32Window(class UIItem* uii);
  ~Win32Window();

  void Draw(class UIItem* uii);

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

  static LRESULT CALLBACK win_proc(HWND   hwnd, UINT   message, WPARAM wParam, LPARAM lParam);
};