#pragma once

// Windows Header Files:
#include <d2d1.h>
#include <windows.h>
#include "FrameBuff.h"
#include "Context.h"



class SystemHandler {
 public:
  SystemHandler(int Width, int Height);
  ~SystemHandler();

  // Register the win & call methods for instantiating drawing res
  HRESULT Initialize();

  // Process and dispatch messages
  void RunMessageLoop();

  // Get Buffer to write
  FBuff *getFBuff();

  // Draw content.
  void SysOutput();

  void GetEventSate(class AppEvent* Event);

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
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                                  LPARAM lParam);

 private:
  FBuff* buff;
  HWND m_hwnd;
  ID2D1Factory* m_pDirect2dFactory;
  ID2D1HwndRenderTarget* m_pRenderTarget;
  ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
  ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
};

void SysOutput(SystemHandler * SH);

void GetEventSate(SystemHandler* SH, class AppEvent* Event);