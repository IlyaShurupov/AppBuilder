#pragma once

// Windows Header Files:
#include <d2d1.h>
#include <windows.h>

#include "FrameBuff.h"


class SystemHandler {
 public:
  SystemHandler(int Width, int Height);
  ~SystemHandler();

  // Register the win & call methods for instantiating drawing res
  HRESULT Initialize();

  // Get Buffer to write
  FBuff* getFBuff();

  // UserInputs
  void getUserInputs(struct UserInputs* user_inputs);

  // Draw Fbuff.
  void SysOutput();

  void consoletoggle();

  bool active();

  void destroy();

 private:
  // Initialize device-independent resources.
  HRESULT CreateDeviceIndependentResources();

  // Initialize device-dependent resources.
  HRESULT CreateDeviceResources();

  // Release device-dependent resource.
  void DiscardDeviceResources();

  // Resize the render target.
  void OnResize(UINT width, UINT height);

  // The windows procedure.
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

 public:
  bool close = false;

 private:
  MSG msg;
  HDC hdcMem;
  HWND m_hwnd;
  FBuff* buff;
  ID2D1Factory* m_pDirect2dFactory;
  ID2D1HwndRenderTarget* m_pRenderTarget;
  ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
  ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
};