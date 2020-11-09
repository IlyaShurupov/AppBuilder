
#include "public/Win32API.h"

#include <d2d1helper.h>
#include <dwrite.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <wchar.h>
#include <wincodec.h>

#include "FrameBuff.h"

#define CREATE_BITMAP(buff, bmp)                                               \
  m_pRenderTarget->CreateBitmap(                                               \
      D2D1::SizeU((UINT32)buff->width, (UINT32)buff->height), buff->Buff,      \
      buff->width * 16,                                                        \
      D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R32G32B32A32_FLOAT, \
                                               D2D1_ALPHA_MODE_IGNORE)),       \
      &bmp)

DemoApp::DemoApp(int Width, int Height) {
  m_hwnd = (NULL);
  m_pDirect2dFactory = (NULL);
  m_pRenderTarget = (NULL);
  m_pLightSlateGrayBrush = (NULL);
  m_pCornflowerBlueBrush = (NULL);
  buff = new FBuff(Width, Height);
}

DemoApp::~DemoApp() {
  SafeRelease(&m_pDirect2dFactory);
  SafeRelease(&m_pRenderTarget);
  SafeRelease(&m_pLightSlateGrayBrush);
  SafeRelease(&m_pCornflowerBlueBrush);
}

void DemoApp::RunMessageLoop() {
  MSG msg;

  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

FBuff *DemoApp::getFBuff() { return buff; }

HRESULT DemoApp::Initialize() {
  HRESULT hr;

  // Initialize device-indpendent resources, such
  // as the Direct2D factory.
  hr = CreateDeviceIndependentResources();

  if (SUCCEEDED(hr)) {
    // Register the window class.
    WNDCLASSEX wcex = {sizeof(WNDCLASSEX)};
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = DemoApp::WndProc;
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

HRESULT DemoApp::CreateDeviceIndependentResources() {
  HRESULT hr = S_OK;

  // Create a Direct2D factory.
  hr =
      D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

  return hr;
}

HRESULT DemoApp::CreateDeviceResources() {
  HRESULT hr = S_OK;

  if (!m_pRenderTarget) {
    RECT rc;
    GetClientRect(m_hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    // Create a Direct2D render target.
    hr = m_pDirect2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size), &m_pRenderTarget);

    if (SUCCEEDED(hr)) {
      // Create a gray brush.
      hr = m_pRenderTarget->CreateSolidColorBrush(
          D2D1::ColorF(D2D1::ColorF::LightSlateGray), &m_pLightSlateGrayBrush);
    }
    if (SUCCEEDED(hr)) {
      // Create a blue brush.
      hr = m_pRenderTarget->CreateSolidColorBrush(
          D2D1::ColorF(D2D1::ColorF::CornflowerBlue), &m_pCornflowerBlueBrush);
    }
  }

  return hr;
}

void DemoApp::DiscardDeviceResources() {
  SafeRelease(&m_pRenderTarget);
  SafeRelease(&m_pLightSlateGrayBrush);
  SafeRelease(&m_pCornflowerBlueBrush);
}

LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam,
                                  LPARAM lParam) {
  LRESULT result = 0;

  if (message == WM_CREATE) {
    LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
    DemoApp *pDemoApp = (DemoApp *)pcs->lpCreateParams;

    ::SetWindowLongPtrW(hwnd, GWLP_USERDATA,
                        reinterpret_cast<LONG_PTR>(pDemoApp));

    result = 1;
  } else {
    DemoApp *pDemoApp = reinterpret_cast<DemoApp *>(
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

        case WM_DISPLAYCHANGE: {
          InvalidateRect(hwnd, NULL, FALSE);
        }
          result = 0;
          wasHandled = true;
          break;

        case WM_PAINT: {
          pDemoApp->OnRender();
          ValidateRect(hwnd, NULL);
        }
          result = 0;
          wasHandled = true;
          break;

        case WM_DESTROY: {
          PostQuitMessage(0);
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

void DemoApp::OnResize(UINT width, UINT height) {
  if (m_pRenderTarget) {
    // Note: This method can fail, but it's okay to ignore the
    // error here, because the error will be returned again
    // the next time EndDraw is called.
    m_pRenderTarget->Resize(D2D1::SizeU(width, height));
  }
}

HRESULT DemoApp::OnRender() {
  HRESULT hr;

  hr = CreateDeviceResources();

  if (!SUCCEEDED(hr)) {
    return hr;
  }

  // Retrieve the size of the render target.
  D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();

  m_pRenderTarget->BeginDraw();
  m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  // Create the bitmap and draw it on the screen
  ID2D1Bitmap *bmp;
  hr = CREATE_BITMAP(buff, bmp);

  if (bmp) {
    D2D1_RECT_F rect = D2D1::RectF(0, 0, buff->width, buff->height);
    m_pRenderTarget->DrawBitmap(bmp, rect);
  }

  hr = m_pRenderTarget->EndDraw();

  if (hr == D2DERR_RECREATE_TARGET) {
    hr = S_OK;
    DiscardDeviceResources();
  }

  return hr;
}

/*
void DemoApp::OnRender() {
  HRESULT hr;

  hr = CreateDeviceResources();

  if (SUCCEEDED(hr)) {
    // Retrieve the size of the render target.
    D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();

    m_pRenderTarget->BeginDraw();
    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

    // Create the bitmap and draw it on the screen
    ID2D1Bitmap *bmp;
    HRESULT hr;
    D2D1_SIZE_U SIZE = D2D1::SizeU((UINT32)buff->width, (UINT32)buff->height);
    UINT32 Pitch = buff->width * sizeof(int) * 4;
    D2D1_BITMAP_PROPERTIES PROPS = D2D1::BitmapProperties(
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));

    hr = m_pRenderTarget->CreateBitmap(SIZE, buff->Buff, Pitch, PROPS, &bmp);

    if (!bmp) {
      return;
    }

    // Draw a bitmap.
    m_pRenderTarget->DrawBitmap(bmp, D2D1::RectF(12, 12, 12 + 800, 12 + 800));

    hr = m_pRenderTarget->EndDraw();

    if (hr == D2DERR_RECREATE_TARGET) {
      hr = S_OK;
      DiscardDeviceResources();
    }
  }
}
*/