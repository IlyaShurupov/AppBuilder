
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

#include "FrameBuff.h"
#include "public/KeyMap.h"


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

bool consolehidden = false;

template <class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease) {
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
  msg = MSG();
  hdcMem = nullptr;
}

SystemHandler::~SystemHandler() {
  SafeRelease(&m_pDirect2dFactory);
  SafeRelease(&m_pRenderTarget);
  SafeRelease(&m_pLightSlateGrayBrush);
  SafeRelease(&m_pCornflowerBlueBrush);
}


HRESULT SystemHandler::Initialize() {
  HRESULT hr;

  ShowWindow(::GetConsoleWindow(), consolehidden ? SW_SHOW : SW_HIDE);
  consolehidden = !consolehidden;

  // Initialize device-indpendent resources, such
  // as the Direct2D factory.
  hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

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
    wcex.lpszClassName = LPCSTR("Gamuncool");

    RegisterClassEx(&wcex);

    // Because the CreateWindow function takes its size in pixels,
    // obtain the system DPI and use it to scale the window size.
    FLOAT dpiX, dpiY;

#pragma warning(push)
#pragma warning(disable : 4996)
    // The factory returns the current system DPI. This is also the value it
    // will use to create its own windows.
    m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);
#pragma warning(pop)

    // Create the window.
    m_hwnd = CreateWindow(LPCSTR("Gamuncool"), LPCSTR("Gamuncool"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                          CW_USEDEFAULT, static_cast<UINT>(ceil(float(buff->width) * dpiX / 96.f)),
                          static_cast<UINT>(ceil(float(buff->height) * dpiY / 96.f)), NULL, NULL,
                          HINST_THISCOMPONENT, this);
    hr = m_hwnd ? S_OK : E_FAIL;
    if (SUCCEEDED(hr)) {
      ShowWindow(m_hwnd, SW_SHOWNORMAL);
      UpdateWindow(m_hwnd);
    }
  }

  CreateDeviceResources();
  m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  hdcMem = CreateCompatibleDC(GetDC(m_hwnd));

  // remove all window styles, check MSDN for details
  SetWindowLong(m_hwnd, GWL_STYLE, 0);  

  ShowWindow(m_hwnd, SW_SHOW); 


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
        D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, size), &m_pRenderTarget);
  }

  return hr;
}

void SystemHandler::DiscardDeviceResources() {
  SafeRelease(&m_pRenderTarget);
  SafeRelease(&m_pLightSlateGrayBrush);
  SafeRelease(&m_pCornflowerBlueBrush);
}

void UpdKeySate(Input& key, bool down) {
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



LRESULT CALLBACK SystemHandler::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  LRESULT result = 0;

  if (message == WM_CREATE) {
    LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
    SystemHandler* pDemoApp = (SystemHandler*)pcs->lpCreateParams;

    ::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pDemoApp));

    result = 1;
  } else {
    SystemHandler* pDemoApp =
        reinterpret_cast<SystemHandler*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));

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

        case WM_CLOSE: {
          pDemoApp->close = true;
          return 0;
        }

        case WM_DESTROY: {
          result = 1;
          wasHandled = true;
          break;
        }
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
    m_pRenderTarget->Resize(D2D1::SizeU(width, height));
    D2D1_SIZE_F TargetSize = m_pRenderTarget->GetSize();
    this->buff->Resize((SCR_UINT)width, (SCR_UINT)height);
  }
}

static HBITMAP CreateBitmapFromPixels(HDC hDC, UINT uWidth, UINT uHeight, UINT uBitsPerPixel, LPVOID pBits) {

  HBITMAP hBitmap = 0;
  if (!uWidth || !uHeight || !uBitsPerPixel) {
    return hBitmap;
  }

  BITMAPINFO bmpInfo = {0};
  bmpInfo.bmiHeader.biBitCount = uBitsPerPixel;
  bmpInfo.bmiHeader.biHeight = uHeight;
  bmpInfo.bmiHeader.biWidth = uWidth;
  bmpInfo.bmiHeader.biPlanes = 1;
  bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  // Pointer to access the pixels of bitmap
  UINT* pPixels = 0;
  hBitmap = CreateDIBSection(hDC, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS, (void**)&pPixels, NULL, 0);

  if (!hBitmap)
    return hBitmap;  // return if invalid bitmaps

  LONG lBmpSize = uWidth * uHeight * (uBitsPerPixel / 8);

  SetBitmapBits(hBitmap, lBmpSize, pBits);

  // memcpy(pPixels, pBits, lBmpSize);

  return hBitmap;
}


void SystemHandler::SysOutput() {

  FBFF_COLOR color = int32_t(0x001b1b1f);
  buff->clear(&color);

  PAINTSTRUCT ps;
  HDC hdcWindow = GetDC(m_hwnd);

  HBITMAP hbmMem = CreateBitmapFromPixels(hdcWindow, buff->width, buff->height, 32, buff->Buff);

  HANDLE hOld = SelectObject(hdcMem, hbmMem);
  BitBlt(hdcWindow, 0, 0, buff->width, buff->height, hdcMem, 0, 0, SRCCOPY);

  DeleteObject(hbmMem);
  ReleaseDC(m_hwnd, hdcWindow);

  SetWindowPos(m_hwnd, HWND_TOP, 10, 10, 100, 100, SWP_NOACTIVATE);
}


void SystemHandler::consoletoggle() {
  ShowWindow(::GetConsoleWindow(), consolehidden ? SW_SHOW : SW_HIDE);
  consolehidden = !consolehidden;
}

bool SystemHandler::active() {
  return GetForegroundWindow() == m_hwnd;
}

void SystemHandler::destroy() {
  KillTimer(m_hwnd, 10);
  PostQuitMessage(0);
}

FBuff* SystemHandler::getFBuff() {
  return buff;
}

void SystemHandler::getUserInputs(UserInputs* user_inputs) {
  POINT cursor;
  UserInputs& usin = *user_inputs;

  usin.PrevCursor = usin.Cursor;

  GetCursorPos(&cursor);
  ScreenToClient(m_hwnd, &cursor);
  usin.Cursor.x = (SCR_UINT)cursor.x;
  usin.Cursor.y = (SCR_UINT)cursor.y;

  SetTimer(m_hwnd, 10, 1000 / 60, (TIMERPROC)NULL);
  GetMessage(&msg, NULL, 0, 0);

  UpdKeySate(usin.LMB, GetKeyState(VK_LBUTTON) & 0x800);
  UpdKeySate(usin.RMB, GetKeyState(VK_RBUTTON) & 0x800);
  UpdKeySate(usin.MMB, GetKeyState(VK_MBUTTON) & 0x800);

  UpdKeySate(usin.K0, GetKeyState('0') & 0x800);
  UpdKeySate(usin.K1, GetKeyState('1') & 0x800);
  UpdKeySate(usin.K2, GetKeyState('2') & 0x800);
  UpdKeySate(usin.K3, GetKeyState('3') & 0x800);
  UpdKeySate(usin.K4, GetKeyState('4') & 0x800);
  UpdKeySate(usin.K5, GetKeyState('5') & 0x800);
  UpdKeySate(usin.K6, GetKeyState('6') & 0x800);
  UpdKeySate(usin.K7, GetKeyState('7') & 0x800);
  UpdKeySate(usin.K8, GetKeyState('8') & 0x800);
  UpdKeySate(usin.K9, GetKeyState('9') & 0x800);

  UpdKeySate(usin.Q, GetKeyState('Q') & 0x800);
  UpdKeySate(usin.W, GetKeyState('W') & 0x800);
  UpdKeySate(usin.E, GetKeyState('E') & 0x800);
  UpdKeySate(usin.R, GetKeyState('R') & 0x800);
  UpdKeySate(usin.T, GetKeyState('T') & 0x800);
  UpdKeySate(usin.Y, GetKeyState('Y') & 0x800);
  UpdKeySate(usin.U, GetKeyState('U') & 0x800);
  UpdKeySate(usin.I, GetKeyState('I') & 0x800);
  UpdKeySate(usin.O, GetKeyState('O') & 0x800);
  UpdKeySate(usin.P, GetKeyState('P') & 0x800);

  UpdKeySate(usin.A, GetKeyState('A') & 0x800);
  UpdKeySate(usin.S, GetKeyState('S') & 0x800);
  UpdKeySate(usin.D, GetKeyState('D') & 0x800);
  UpdKeySate(usin.F, GetKeyState('F') & 0x800);
  UpdKeySate(usin.G, GetKeyState('G') & 0x800);
  UpdKeySate(usin.H, GetKeyState('H') & 0x800);
  UpdKeySate(usin.J, GetKeyState('J') & 0x800);
  UpdKeySate(usin.K, GetKeyState('K') & 0x800);
  UpdKeySate(usin.L, GetKeyState('L') & 0x800);

  UpdKeySate(usin.Z, GetKeyState('Z') & 0x800);
  UpdKeySate(usin.X, GetKeyState('X') & 0x800);
  UpdKeySate(usin.C, GetKeyState('C') & 0x800);
  UpdKeySate(usin.V, GetKeyState('V') & 0x800);
  UpdKeySate(usin.B, GetKeyState('B') & 0x800);
  UpdKeySate(usin.N, GetKeyState('N') & 0x800);
  UpdKeySate(usin.M, GetKeyState('M') & 0x800);

  UpdKeySate(usin.SPACE, GetKeyState(VK_SPACE) & 0x800);
  UpdKeySate(usin.ENTER, GetKeyState(VK_SEPARATOR) & 0x800);
  UpdKeySate(usin.DEL, GetKeyState(VK_DELETE) & 0x800);
  UpdKeySate(usin.EREASE, GetKeyState(VK_BACK) & 0x800);
  UpdKeySate(usin.ESCAPE, GetKeyState(VK_ESCAPE) & 0x800);
  UpdKeySate(usin.TAB, GetKeyState(VK_TAB) & 0x800);

  UpdKeySate(usin.SHIFT_L, GetKeyState(VK_LSHIFT) & 0x800);
  UpdKeySate(usin.SHIFT_R, GetKeyState(VK_RSHIFT) & 0x800);
  UpdKeySate(usin.CTR_L, GetKeyState(VK_LCONTROL) & 0x800);
  UpdKeySate(usin.CTR_R, GetKeyState(VK_RCONTROL) & 0x800);
  UpdKeySate(usin.ALT_L, GetKeyState(VK_MENU) & 0x800);
  UpdKeySate(usin.ALT_R, GetKeyState(VK_MENU) & 0x800);

  usin.SYS_DESTROY_COMMAND.state = close ? InputState::PRESSED : InputState::NONE;

  // USRINPUT_DECL(ARROW_UP);
  // USRINPUT_DECL(ARROW_DOWN);
  // USRINPUT_DECL(ARROW_LEFT);
  // USRINPUT_DECL(ARROW_RIGHT);

  /*
  UpdKeySate(usin.D, GetKeyState('D') & 0x800);
  //...
  UpdKeySate(usin.LMB, GetKeyState(VK_LBUTTON) & 0x800);
  UpdKeySate(usin.RMB, GetKeyState(VK_RBUTTON) & 0x800);
  UpdKeySate(usin.LMB, GetKeyState(VK_MBUTTON) & 0x800);

  */
  TranslateMessage(&msg);
  DispatchMessage(&msg);
}