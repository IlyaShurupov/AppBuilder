#pragma once

#include "FrameBuff.h"

struct HWND_;
typedef struct HWND_* HWND;


struct WindowWin32 {

  WindowWin32(Rect<SCR_UINT>& rect);
  ~WindowWin32();


  // UserInputs
  void getUserInputs(struct UInputs* user_inputs, SCR_UINT scry);

  // Draw Fbuff.
  void SysOutput(FBuff<RGBAf>* buff);
  void SysOutput(FBuff<RGBA_32>* buff);

  void ProcSysEvents();

  void ShowInitializedWindow();
  bool active();
  void getScreenSize(vec2<SCR_UINT>& rect);
  void getRect(Rect<SCR_UINT>& rect, SCR_UINT scry);
  void setRect(Rect<float>& rect, SCR_UINT scry);
  void SetIcon(struct Str& stricon);
  void drawRect(Rect<SCR_UINT>& rect);

 private:

 static __int64 __stdcall win_proc(HWND hwnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);

  void* hWindowIcon;
  void* hWindowIconBig;
  void* hdcMem;
  void* m_hwnd;
  void* m_pDirect2dFactory;

  void* msg;
};

  void consoletoggle();