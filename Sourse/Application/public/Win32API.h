#pragma once

// Windows Header Files:
#include <d2d1.h>
#include <windows.h>

#include "FrameBuff.h"

#define DKS \
  { KeyState::EVENT_NONE }

enum class KeyState {
  EVENT_NONE = 0,
  HOLD,
  PRESSED,
  RELEASED,
};

struct UserInputs {
  KeyState LMB DKS;
  KeyState RMB DKS;
  KeyState MMB DKS;

  KeyState A DKS;
  KeyState B DKS;
  KeyState C DKS;
  KeyState D DKS;
  KeyState ENTER DKS;
  KeyState SPACE DKS;
  KeyState KEY_1 DKS;
  KeyState KEY_2 DKS;
  KeyState KEY_3 DKS;
  // ...

  vec2<SCR_UINT> Cursor;
  vec2<SCR_UINT> PrevCursor;

  bool IsEvent = true;
};

class SystemHandler {
 public:
  SystemHandler(int Width, int Height);
  ~SystemHandler();

  // Register the win & call methods for instantiating drawing res
  HRESULT Initialize();

  // Get Buffer to write
  FBuff* getFBuff();

  // UserInputs
  UserInputs* getUserInputs();

  // Draw Fbuff.
  void SysOutput();

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
  MSG msg;
  HWND m_hwnd;
  UserInputs user_inputs;
  FBuff* buff;
  ID2D1Factory* m_pDirect2dFactory;
  ID2D1HwndRenderTarget* m_pRenderTarget;
  ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
  ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
};