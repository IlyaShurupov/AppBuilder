
#include <conio.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wincodec.h>

#include "d2d1_1.h"
#include "wingdi.h"
#include <d2d1.h>
#include <windows.h>
#include "FrameBuff.h"
#include "UI/UInputsMap.h"
#include "UI/UInputs.h"
#include "UI/UInterface.h"
#include "Win32Window.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#define TRANSPARENTCY
#undef TRANSPARENTCY

template <class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease) {
  if (*ppInterfaceToRelease != NULL) {
    (*ppInterfaceToRelease)->Release();

    (*ppInterfaceToRelease) = NULL;
  }
}

void SetRect(Rect<float>& rect, SCR_UINT scry, void* m_hwnd) {

  Rect<SCR_UINT> cprect;
  cprect.size.assign(rect.size.x, rect.size.y);
  cprect.pos.assign(rect.pos.x, rect.pos.y);

  cprect.inv_y(scry);

  SetWindowPos((HWND)m_hwnd, HWND_TOP, cprect.pos.x, cprect.pos.y, cprect.size.x, cprect.size.y, SWP_DRAWFRAME);
}

void drawbmp(HWND hwnd, HBITMAP hbmp) {

  // get the size of the bitmap
  BITMAP bm;
  GetObject(hbmp, sizeof(bm), &bm);
  SIZE size = {bm.bmWidth, bm.bmHeight};

  // create a memory DC holding the splash bitmap
  HDC hdcScr = GetDC(NULL);
  HDC hdcMem = CreateCompatibleDC(hdcScr);
  HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcMem, hbmp);

  // use the source image's alpha channel for blending
  BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

  LPRECT wrect_p = &RECT();
  GetWindowRect(hwnd, wrect_p);

  POINT pos;
  pos.y = wrect_p->top;
  pos.x = wrect_p->left;

  POINT ptZero = {0};

  // paint the window (in the right location) with the alpha-blended bitmap
  UpdateLayeredWindow(hwnd, hdcScr, &pos, &size, hdcMem, &ptZero, RGB(0, 0, 0), &bf, 2);

  // DELETE_DBG() temporary objects
  SelectObject(hdcMem, hbmpOld);
  DeleteDC(hdcMem);
  ReleaseDC(NULL, hdcScr);
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

void Draw_RGBA_32(FBuff<RGBA_32>* buff, void* m_hwnd, void* hdcMem) {

  HDC hdcWindow = GetDC((HWND)m_hwnd);

  HBITMAP hbmMem = CreateBitmapFromPixels(hdcWindow, buff->size.x, buff->size.y, 32, buff->pxls);

#ifdef TRANSPARENTCY
  drawbmp(m_hwnd, hbmMem);
#else
  SelectObject((HDC)hdcMem, hbmMem);
  BitBlt(hdcWindow, 0, 0, buff->size.x, buff->size.y, (HDC)hdcMem, 0, 0, SRCCOPY);
#endif

  DeleteObject(hbmMem);
  ReleaseDC((HWND)m_hwnd, hdcWindow);
}

Win32Window::Win32Window(UIItem* uii) {

  Rect<SCR_UINT> rect;
  rect.size.assign(uii->rect.size.x, uii->rect.size.y);
  rect.pos.assign(uii->rect.pos.x, uii->rect.pos.y);
  scr_y = GetDeviceCaps(GetDC(NULL), VERTRES);
  rect.inv_y(scr_y);

  editflag = 0;
  hWindowIcon = nullptr;
  hWindowIconBig = nullptr;
  hdcMem = nullptr;
  m_hwnd = nullptr;
  m_pDirect2dFactory = nullptr;

  msg = new MSG();

  HRESULT hr;

  rect.inv_y(GetDeviceCaps(GetDC(NULL), VERTRES));

  ID2D1Factory* f = ((ID2D1Factory*)m_pDirect2dFactory);
  hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &f);
  m_pDirect2dFactory = (void*)f;

  if (SUCCEEDED(hr)) {

    // Register the window class.
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Win32Window::win_proc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(LONG_PTR);
    wcex.hInstance = HINST_THISCOMPONENT;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
    wcex.lpszClassName = LPCSTR("Gamuncool");

    RegisterClassEx(&wcex);

    FLOAT dpiX, dpiY;

    #pragma warning(push)
    #pragma warning(disable : 4996)
    ((ID2D1Factory*)m_pDirect2dFactory)->GetDesktopDpi(&dpiX, &dpiY);
    #pragma warning(pop)

    // Create the window.
    LPCSTR name = LPCSTR("Gamuncool");
    UINT sizex = static_cast<UINT>(ceil(float(rect.size.x) * dpiX / 96.f));
    UINT sizey = static_cast<UINT>(ceil(float(rect.size.y) * dpiY / 96.f));
    m_hwnd = (HWND)CreateWindow(name, name, WS_POPUP, rect.pos.x, rect.pos.y, sizex, sizey, NULL, NULL,
                                HINST_THISCOMPONENT, this);

    hr = (HWND)m_hwnd ? S_OK : E_FAIL;
    if (SUCCEEDED(hr)) {

      #ifdef TRANSPARENTCY
      SetWindowLong(m_hwnd, GWL_EXSTYLE, GetWindowLong(m_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
      #else
      SetWindowLong((HWND)m_hwnd, GWL_EXSTYLE, 0);
      #endif

      hdcMem = (HDC)CreateCompatibleDC(GetDC((HWND)m_hwnd));

      SetMenu((HWND)m_hwnd, NULL);
    }
  }

  rect.inv_y(GetDeviceCaps(GetDC(NULL), VERTRES));
  ShowWindow((HWND)m_hwnd, SW_SHOWNORMAL);
  UpdateWindow((HWND)m_hwnd);
}

void Win32Window::Draw(UIItem* uii) {
  
  ProcSysEvents();

  if (!(uii->rect == prevrec)) {
    SetRect(uii->rect, (SCR_UINT)scr_y, m_hwnd);
  }
  
  Draw_RGBA_32(uii->buff, m_hwnd, hdcMem);

  prevrec = uii->rect;
}

bool Win32Window::active() {
  return GetForegroundWindow() == (HWND)m_hwnd;
}

void Win32Window::ProcSysEvents() {
  while (PeekMessage(&(*(MSG*)msg), (HWND)m_hwnd, 0, 0, PM_REMOVE)) {
    DispatchMessage(&(*(MSG*)msg));
    TranslateMessage(&(*(MSG*)msg));
  }
}

void Win32Window::SetIcon(Str& stricon) {
  if (hWindowIcon != NULL)
    DestroyIcon((HICON)hWindowIcon);
  if (hWindowIconBig != NULL)
    DestroyIcon((HICON)hWindowIconBig);
  if (stricon == "") {
    SendMessage((HWND)m_hwnd, WM_SETICON, ICON_SMALL, (LPARAM)NULL);
    SendMessage((HWND)m_hwnd, WM_SETICON, ICON_BIG, (LPARAM)NULL);
  } else {
    hWindowIcon = (HICON)LoadImage(NULL, stricon.str, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
    hWindowIconBig = (HICON)LoadImage(NULL, stricon.str, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    SendMessage((HWND)m_hwnd, WM_SETICON, ICON_SMALL, (LPARAM)(HICON)hWindowIcon);
    SendMessage((HWND)m_hwnd, WM_SETICON, ICON_BIG, (LPARAM)(HICON)hWindowIconBig);
  }
}


Win32Window::~Win32Window() {
  KillTimer((HWND)m_hwnd, 10);
  ID2D1Factory* f = ((ID2D1Factory*)m_pDirect2dFactory);
  SafeRelease(&f);
}

__int64 __stdcall Win32Window::win_proc(HWND hwnd, unsigned int message, unsigned __int64 wParam, __int64 lParam) {

  LRESULT result = 0;

  if (message == WM_CREATE) {
    LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
    Win32Window* pDemoApp = (Win32Window*)pcs->lpCreateParams;
    SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pDemoApp));
    result = 1;

  } else {

    LONG_PTR pDemoAppptr = static_cast<LONG_PTR>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    Win32Window* pDemoApp = reinterpret_cast<Win32Window*>(pDemoAppptr);

    bool wasHandled = false;

    if (pDemoApp) {

      switch (message) {

        case WM_SIZE:
          wasHandled = true;
          break;

        case WM_CLOSE: {
          result = 1;
          wasHandled = true;
          return 0;
        }

        case WM_DESTROY: {
          result = 1;
          wasHandled = true;
          break;
        }

        case WM_ACTIVATE: {
        }
      }
    }

    if (!wasHandled) {
      result = DefWindowProc(hwnd, message, wParam, lParam);
    }
  }

  return result;
}

/*
void Win32Window::ShowInitializedWindow() {
  ShowWindow((HWND)m_hwnd, SW_SHOWNORMAL);
  UpdateWindow((HWND)m_hwnd);
}
*/

/*
// very slow!!!!!
void Win32Window::getScreenSize(vec2<SCR_UINT>& rect) {
  rect.y = GetDeviceCaps(GetDC(NULL), VERTRES);
  rect.x = GetDeviceCaps(GetDC(NULL), HORZRES);
}
*/

/*
void Win32Window::getRect(Rect<SCR_UINT>& rect, SCR_UINT scry) {

  LPRECT wrect_p = &RECT();
  GetWindowRect((HWND)m_hwnd, wrect_p);

  rect.pos.x = wrect_p->left;
  rect.pos.y = wrect_p->top;

  rect.size.y = wrect_p->bottom - wrect_p->top;
  rect.size.x = wrect_p->right - wrect_p->left;

  // vec2<SCR_UINT> scr_size;
  // getScreenSize(scr_size);
  rect.inv_y(scry);
}
*/


  /*
void Win32Window::drawRect(Rect<SCR_UINT>& rect) {
HDC hdc = BeginPaint(m_hwnd, &ps);
//hdc = BeginPaint(m_hWnd, &ps);
//    Initializing original object
HGDIOBJ original = NULL;

//    Saving the original object
original = SelectObject(hdc, GetStockObject(DC_PEN));

//    Rectangle function is defined as...
//    BOOL Rectangle(hdc, xLeft, yTop, yRight, yBottom);

//    Drawing a rectangle with just a black pen
//    The black pen object is selected and sent to the current device context
//  The default brush is WHITE_BRUSH
SelectObject(hdc, GetStockObject(BLACK_PEN));
Rectangle(hdc, 0, 0, 200, 200);

//    Select DC_PEN so you can change the color of the pen with
//    COLORREF SetDCPenColor(HDC hdc, COLORREF color)
SelectObject(hdc, GetStockObject(DC_PEN));

//    Select DC_BRUSH so you can change the brush color from the
//    default WHITE_BRUSH to any other color
SelectObject(hdc, GetStockObject(DC_BRUSH));

//    Set the DC Brush to Red
//    The RGB macro is declared in "Windowsx.h"
SetDCBrushColor(hdc, RGB(255, 0, 0));

//    Set the Pen to Blue
SetDCPenColor(hdc, RGB(0, 0, 255));

//    Drawing a rectangle with the current Device Context
Rectangle(hdc, 100, 300, 200, 400);

//    Changing the color of the brush to Green
SetDCBrushColor(hdc, RGB(0, 255, 0));
Rectangle(hdc, 300, 150, 500, 300);

//    Restoring the original object
SelectObject(hdc, original);
}
*/

/*
void Win32Window::consoletoggle() {
  
}
*/