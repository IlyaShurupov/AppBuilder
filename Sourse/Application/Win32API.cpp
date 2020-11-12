
#include "public/Win32API.h"

#include <d2d1helper.h>
#include <dwrite.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wincodec.h>

//#include "public/Window.h"
#include "FrameBuff.h"
#include "public/KeyMap.h"

#define CREATE_BITMAP(buff, bmp)                                               \
  m_pRenderTarget->CreateBitmap(                                               \
      D2D1::SizeU((UINT32)buff->width, (UINT32)buff->height), buff->Buff,      \
      buff->width * 16,                                                        \
      D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R32G32B32A32_FLOAT, \
                                               D2D1_ALPHA_MODE_IGNORE)),       \
      &bmp)

#ifndef Assert
#if defined(DEBUG) || defined(_DEBUG)
#define Assert(b)                             \
  do {                                        \
    if (!(b)) {                               \
      OutputDebugStringA("Assert: " #b "\n"); \
    }                                         \
  } while (0)
#else
#define Assert(b)
#endif  // DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

template <class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease) {
  if (*ppInterfaceToRelease != NULL) {
    (*ppInterfaceToRelease)->Release();

    (*ppInterfaceToRelease) = NULL;
  }
}

SystemHandler::SystemHandler(int Width, int Height) {
  m_hwnd = (NULL);
  m_pDirect2dFactory = (NULL);
  m_pRenderTarget = (NULL);
  m_pLightSlateGrayBrush = (NULL);
  m_pCornflowerBlueBrush = (NULL);
  buff = DBG_NEW FBuff(Width, Height);
}

SystemHandler::~SystemHandler() {
  SafeRelease(&m_pDirect2dFactory);
  SafeRelease(&m_pRenderTarget);
  SafeRelease(&m_pLightSlateGrayBrush);
  SafeRelease(&m_pCornflowerBlueBrush);
}

FBuff *SystemHandler::getFBuff() { return buff; }

HRESULT SystemHandler::Initialize() {
  HRESULT hr;

  // Initialize device-indpendent resources, such
  // as the Direct2D factory.
  hr = CreateDeviceIndependentResources();

  if (SUCCEEDED(hr)) {
    // Register the window class.
    WNDCLASSEX wcex = {sizeof(WNDCLASSEX)};
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = SystemHandler::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(LONG_PTR);
    wcex.hInstance = HINST_THISCOMPONENT;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
    wcex.lpszClassName = LPCSTR("D2DDemoApp");

    RegisterClassEx(&wcex);

    // Because the CreateWindow function takes its size in pixels,
    // obtain the system DPI and use it to scale the window size.
    FLOAT dpiX, dpiY;

    // The factory returns the current system DPI. This is also the value it
    // will use to create its own windows.
    m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

    // Create the window.
    m_hwnd =
        CreateWindow(LPCSTR("D2DDemoApp"), LPCSTR("D2DDemoApp"),
                     WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                     static_cast<UINT>(ceil(float(buff->width) * dpiX / 96.f)),
                     static_cast<UINT>(ceil(float(buff->height) * dpiY / 96.f)),
                     NULL, NULL, HINST_THISCOMPONENT, this);
    hr = m_hwnd ? S_OK : E_FAIL;
    if (SUCCEEDED(hr)) {
      ShowWindow(m_hwnd, SW_SHOWNORMAL);
      UpdateWindow(m_hwnd);
    }
  }

  return hr;
}

HRESULT SystemHandler::CreateDeviceIndependentResources() {
  HRESULT hr = S_OK;

  // Create a Direct2D factory.
  hr =
      D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

  return hr;
}

HRESULT SystemHandler::CreateDeviceResources() {
  HRESULT hr = S_OK;

  if (!m_pRenderTarget) {
    RECT rc;
    GetClientRect(m_hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    // Create a Direct2D render target.
    hr = m_pDirect2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size), &m_pRenderTarget);
  }

  return hr;
}

void SystemHandler::DiscardDeviceResources() {
  SafeRelease(&m_pRenderTarget);
  SafeRelease(&m_pLightSlateGrayBrush);
  SafeRelease(&m_pCornflowerBlueBrush);
}


void UpdKeySate(Input &key, bool down) {
  if ((int)key.state == (int)down) {
    return;
  }

  if (key.state == InputState::NONE) {
    key.state = InputState::PRESSED;
  
  } else if (key.state == InputState::HOLD) {
    key.state = InputState::RELEASED;
  } else {
    key.state = InputState(down);
  }
}

void SystemHandler::getUserInputs(UserInputs *user_inputs) {

  POINT cursor;
  UserInputs &usin = *user_inputs;

  usin.PrevCursor = usin.Cursor;

  GetCursorPos(&cursor);
  ScreenToClient(m_hwnd, &cursor);
  usin.Cursor.x = (SCR_UINT)cursor.x;
  usin.Cursor.y = (SCR_UINT)cursor.y;

  SetTimer(m_hwnd, 10, 1000 / 60, (TIMERPROC)NULL);
  GetMessage(&msg, NULL, 0, 0);

  char A[5] = {'A', '0', '0', '0', '0'};
  UpdKeySate(usin.A, GetKeyState('A') & 0x800);
  
 
  UpdKeySate(usin.B, GetKeyState('B') & 0x800);
  UpdKeySate(usin.C, GetKeyState('C') & 0x800);

  /*
  UpdKeySate(usin.D, GetKeyState('D') & 0x800);
  //...
  UpdKeySate(usin.LMB, GetKeyState(VK_LBUTTON) & 0x800);
  UpdKeySate(usin.RMB, GetKeyState(VK_RBUTTON) & 0x800);
  UpdKeySate(usin.LMB, GetKeyState(VK_MBUTTON) & 0x800);
  */
}

LRESULT CALLBACK SystemHandler::WndProc(HWND hwnd, UINT message, WPARAM wParam,
                                        LPARAM lParam) {
  LRESULT result = 0;

  if (message == WM_CREATE) {
    LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
    SystemHandler *pDemoApp = (SystemHandler *)pcs->lpCreateParams;

    ::SetWindowLongPtrW(hwnd, GWLP_USERDATA,
                        reinterpret_cast<LONG_PTR>(pDemoApp));

    result = 1;
  } else {
    SystemHandler *pDemoApp = reinterpret_cast<SystemHandler *>(
        static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));

    bool wasHandled = false;

    if (pDemoApp) {
      switch (message) {
        case WM_SIZE: {
          UINT width = LOWORD(lParam);
          UINT height = HIWORD(lParam);
          pDemoApp->OnResize(width, height);
        }
          result = 0;
          wasHandled = true;
          break;

        case WM_TIMER:
          break;

        case WM_DESTROY: {
          PostQuitMessage(0);
          KillTimer(hwnd, 10);
        }
          result = 1;
          wasHandled = true;
          break;
      }
    }

    if (!wasHandled) {
      result = DefWindowProc(hwnd, message, wParam, lParam);
    }
  }

  return result;
}

void SystemHandler::OnResize(UINT width, UINT height) {
  if (m_pRenderTarget) {
    // Note: This method can fail, but it's okay to ignore the
    // error here, because the error will be returned again
    // the next time EndDraw is called.
    m_pRenderTarget->Resize(D2D1::SizeU(width, height));
    D2D1_SIZE_F TargetSize = m_pRenderTarget->GetSize();
    this->buff->Resize((SCR_UINT)width, (SCR_UINT)height);
  }
}

void SystemHandler::SysOutput() {
  HRESULT hr;

  hr = CreateDeviceResources();

  if (!SUCCEEDED(hr)) {
    return;
  }

  // Retrieve the size of the render target.
  D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();

  m_pRenderTarget->BeginDraw();
  m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  // Create the bitmap and draw it on the screen
  ID2D1Bitmap *bmp;
  hr = CREATE_BITMAP(buff, bmp);

  if (bmp) {
    D2D1_RECT_F rect = D2D1::RectF(10.f, 10.f, float(buff->width - 10),
                                   float(buff->height - 10));
    m_pRenderTarget->DrawBitmap(bmp, rect);
  }

  hr = m_pRenderTarget->EndDraw();

  if (hr == D2DERR_RECREATE_TARGET) {
    hr = S_OK;
    DiscardDeviceResources();
  }

  TranslateMessage(&msg);
  DispatchMessage(&msg);
  return;
}
