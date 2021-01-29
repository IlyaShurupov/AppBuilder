#pragma once

#include "FrameBuff.h"
#include <d2d1.h>
#include <windows.h>


class SystemHandler {
 public:
  SystemHandler(Rect<SCR_UINT>& rect);
  ~SystemHandler();

  // Register the win & call methods for instantiating drawing res
  HRESULT Initialize(Rect<SCR_UINT>& rect);
  void ShowInitializedWindow();

  // UserInputs
  void getUserInputs(struct UserInputs* user_inputs, SCR_UINT scry);

  // Draw Fbuff.
  void SysOutput(FBuff<RGBAf>* buff);
  void SysOutput(FBuff<RGBA_32>* buff);

  void consoletoggle();

  bool active();

  void getScreenSize(vec2<SCR_UINT>& rect);
  void getRect(Rect<SCR_UINT>& rect, SCR_UINT scry);
  void setRect(Rect<SCR_UINT>& rect, SCR_UINT scry);

  void SetIcon(struct Str& stricon);

  void drawRect(Rect<SCR_UINT>& rect);

 private:

  // The windows procedure.
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

 public:
  bool close = false;

 private:
   void* hWindowIcon;
   void* hWindowIconBig;
   void* hdcMem;
   void* m_hwnd;
   void* m_pDirect2dFactory;

  MSG msg;
};