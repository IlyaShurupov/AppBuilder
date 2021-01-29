#pragma once

#include "FrameBuff.h"

struct HWND_;
typedef struct HWND_* HWND;

struct SystemHandler {

  bool close = false;

  SystemHandler(Rect<SCR_UINT>& rect);
  ~SystemHandler();

  // UserInputs
  void getUserInputs(struct UserInputs* user_inputs, SCR_UINT scry);

  // Draw Fbuff.
  void SysOutput(FBuff<RGBAf>* buff);
  void SysOutput(FBuff<RGBA_32>* buff);


  void ShowInitializedWindow();
  void consoletoggle();
  bool active();
  void getScreenSize(vec2<SCR_UINT>& rect);
  void getRect(Rect<SCR_UINT>& rect, SCR_UINT scry);
  void setRect(Rect<SCR_UINT>& rect, SCR_UINT scry);
  void SetIcon(struct Str& stricon);
  void drawRect(Rect<SCR_UINT>& rect);

 private:

 static __int64 __stdcall SystemHandler::win_proc(HWND hwnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);

  void* hWindowIcon;
  void* hWindowIconBig;
  void* hdcMem;
  void* m_hwnd;
  void* m_pDirect2dFactory;

  void* msg;
};