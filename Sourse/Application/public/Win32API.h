#pragma once

#include "FrameBuff.h"

struct HWND_;
typedef struct HWND_* HWND;

struct SystemHandler {

  SystemHandler(Rect<SCR_UINT>& rect, struct Str& stricon);
  ~SystemHandler();

  // UserInputs
  void getUserInputs(struct UserInputs* user_inputs, SCR_UINT scry);

  // Draw Fbuff.
  void SysOutput(FBuff<RGBAf>* buff);
  void SysOutput(FBuff<RGBA_32>* buff);

  bool active();
  void consoletoggle();
  void getScreenSize(vec2<SCR_UINT>& rect);
  void setRect(Rect<SCR_UINT>& rect, SCR_UINT scry);

  void getRect(Rect<SCR_UINT>& rect, SCR_UINT scry);
  void ShowInitializedWindow();
  void SetIcon(struct Str& stricon);
  void drawRect(Rect<SCR_UINT>& rect);

  bool close = false;

 private:

  static __int64 __stdcall SystemHandler::win_proc(HWND hwnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);
 
  void* hWindowIcon;
  void* hWindowIconBig;
  void* hdcMem;
  void* m_hwnd;
  void* m_pDirect2dFactory;
  void* msg;
};