
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

SystemHandler::SystemHandler() {
  m_hwnd = (NULL);
  m_pDirect2dFactory = (NULL);
  msg = MSG();
  hdcMem = nullptr;
}

SystemHandler::~SystemHandler() {
  SafeRelease(&m_pDirect2dFactory);
}


HRESULT SystemHandler::Initialize(vec2<SCR_UINT>& size) {
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

    // The factory returns the current system DPI. This is also the value it
    // will use to create its own windows.
#pragma warning(push)
#pragma warning(disable : 4996)
    m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);
#pragma warning(pop)

    // Create the window.
    LPCSTR name = LPCSTR("Gamuncool");
    UINT sizex = static_cast<UINT>(ceil(float(size.x) * dpiX / 96.f));
    UINT sizey = static_cast<UINT>(ceil(float(size.y) * dpiY / 96.f));
    m_hwnd = CreateWindow(name, name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, sizex, sizey, NULL,
                          NULL, HINST_THISCOMPONENT, this);

    hr = m_hwnd ? S_OK : E_FAIL;
    if (SUCCEEDED(hr)) {

      SetWindowLong(m_hwnd, GWL_STYLE, 0);
      hdcMem = CreateCompatibleDC(GetDC(m_hwnd));
      ShowWindow(m_hwnd, SW_SHOWNORMAL);

      UpdateWindow(m_hwnd);
    }
  }

  return hr;
}

LRESULT CALLBACK SystemHandler::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  LRESULT result = 0;

  if (message == WM_CREATE) {
    LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
    SystemHandler* pDemoApp = (SystemHandler*)pcs->lpCreateParams;
    SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pDemoApp));
    result = 1;

  } else {

    LONG_PTR pDemoAppptr = static_cast<LONG_PTR>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    SystemHandler* pDemoApp = reinterpret_cast<SystemHandler*>(pDemoAppptr);

    bool wasHandled = false;

    if (pDemoApp) {

      switch (message) {

        case WM_CLOSE: {
          pDemoApp->close = true;
          return 0;
        }

        case WM_DESTROY: {
          result = 1;
          wasHandled = true;
          break;
        }

        //default: {
          //wasHandled = true;
          //break;
        //}
      }
    }

    if (!wasHandled) {
      result = DefWindowProc(hwnd, message, wParam, lParam);
    }
  }

  return result;
}

static HBITMAP CreateBitmapFromPixels(HDC hDC, UINT uWidth, UINT uHeight, UINT uBitsPerPixel, LPVOID pBits) {

  HBITMAP hBitmap = 0;
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

  // SetBitmapBits(hBitmap, lBmpSize, pBits);

  memcpy(pPixels, pBits, lBmpSize);

  return hBitmap;
}


void SystemHandler::SysOutput(FBuff* buff) {

  FBFF_COLOR color = int32_t(0x001b1b1f);
  buff->clear(&color);

  HDC hdcWindow = GetDC(m_hwnd);

  HBITMAP hbmMem = CreateBitmapFromPixels(hdcWindow, buff->width, buff->height, 32, buff->Buff);

  HANDLE hOld = SelectObject(hdcMem, hbmMem);
  BitBlt(hdcWindow, 0, 0, buff->width, buff->height, hdcMem, 0, 0, SRCCOPY);

  DeleteObject(hbmMem);
  ReleaseDC(m_hwnd, hdcWindow);

  //Sleep(100);
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

// very slow!!!!!
void SystemHandler::getScreenSize(vec2<SCR_UINT>& rect) {
  rect.y = GetDeviceCaps(GetDC(NULL), VERTRES);
  rect.x = GetDeviceCaps(GetDC(NULL), HORZRES);
}

void SystemHandler::getRect(Rect<SCR_UINT>& rect, SCR_UINT scry) {

  LPRECT wrect_p = &RECT();
  GetWindowRect(m_hwnd, wrect_p);

  rect.pos.x = wrect_p->left;
  rect.pos.y = wrect_p->top;

  rect.size.y = wrect_p->bottom - wrect_p->top;
  rect.size.x = wrect_p->right - wrect_p->left;

  //vec2<SCR_UINT> scr_size;
  //getScreenSize(scr_size);
  rect.inv_y(scry);
}

void SystemHandler::setRect(Rect<SCR_UINT>& rect, SCR_UINT scry) {
  Rect<SCR_UINT> cprect = rect;
  vec2<SCR_UINT> scr_size;


  cprect.inv_y(scry);

  SetWindowPos(m_hwnd, HWND_TOP, cprect.pos.x, cprect.pos.y, cprect.size.x, cprect.size.y, SWP_NOACTIVATE);
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

void SystemHandler::getUserInputs(UserInputs* user_inputs, SCR_UINT scry) {

  POINT cursor;
  UserInputs& usin = *user_inputs;

  usin.PrevCursor = usin.Cursor;

  GetCursorPos(&cursor);
  cursor.y = scry - cursor.y;
  usin.Cursor.x = (SCR_UINT)cursor.x;
  usin.Cursor.y = (SCR_UINT)(cursor.y);

  Rect<SCR_UINT> rect;
  getRect(rect, scry);

  rect.pos.x;
  usin.Cursor = usin.Cursor - rect.pos;
  

  usin.Cdelta.x = usin.Cursor.x - usin.PrevCursor.x;
  usin.Cdelta.y = usin.Cursor.y - usin.PrevCursor.y;


  SetTimer(m_hwnd, 10, 1000/60, (TIMERPROC)NULL);

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

  UpdKeySate(usin.WIN_KEY, GetKeyState(VK_LWIN) & 0x800);
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

  TranslateMessage(&msg);
  DispatchMessage(&msg);
}

void SystemHandler::SetIcon(std::string stricon) {
  if (hWindowIcon != NULL)
    DestroyIcon(hWindowIcon);
  if (hWindowIconBig != NULL)
    DestroyIcon(hWindowIconBig);
  if (stricon == "") {
    SendMessage(m_hwnd, WM_SETICON, ICON_SMALL, (LPARAM)NULL);
    SendMessage(m_hwnd, WM_SETICON, ICON_BIG, (LPARAM)NULL);
  } else {
    hWindowIcon = (HICON)LoadImage(NULL, stricon.c_str(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
    hWindowIconBig = (HICON)LoadImage(NULL, stricon.c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    SendMessage(m_hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hWindowIcon);
    SendMessage(m_hwnd, WM_SETICON, ICON_BIG, (LPARAM)hWindowIconBig);
  }
}