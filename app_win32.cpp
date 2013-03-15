#include "app.h"

#ifdef WIN32

void Application::swapBuffers() {
   SwapBuffers(hdc);
}

void Application::initialize() {
   WNDCLASSEX wcx;
   wcx.cbSize = sizeof(wcx);
   wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
   wcx.lpfnWndProc = DefWindowProc;
   wcx.cbClsExtra = 0;
   wcx.cbWndExtra = sizeof(Application*);
   wcx.hInstance = GetModuleHandle(NULL);
   wcx.hIcon = NULL;
   wcx.hCursor = NULL;
   wcx.hbrBackground = NULL;
   wcx.lpszMenuName = NULL;
   wcx.lpszClassName = "WindowClass";
   wcx.hIconSm = NULL;

   RegisterClassEx(&wcx);
   
   hwnd = CreateWindowEx(0,
         "WindowClass",
         title,
         WS_OVERLAPPEDWINDOW,
         CW_USEDEFAULT,
         CW_USEDEFAULT,
         width,
         height,
         HWND_DESKTOP,
         (HMENU) NULL,
         GetModuleHandle(NULL),
         (LPVOID)NULL);

   SetWindowLongPtr(hwnd, 0, (LONG_PTR)this);
   SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)MainWndProc);
   
   PIXELFORMATDESCRIPTOR pfd = {0};

   pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
   pfd.nVersion = 1;
   pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
   pfd.iPixelType = PFD_TYPE_RGBA;
   pfd.cColorBits = 32;
   pfd.cDepthBits = 32;
   pfd.cDepthBits = 32;
   pfd.cAccumBits = 32;
   pfd.cStencilBits = 32;
   pfd.cAlphaBits = 32;
   pfd.iLayerType = PFD_MAIN_PLANE;

   hdc = GetWindowDC(hwnd);

   int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
   BOOL bResult = SetPixelFormat(hdc, nPixelFormat, &pfd);

   HGLRC tempContext = wglCreateContext(hdc);
   wglMakeCurrent(hdc, tempContext);

   //                      if(wglewIsSupported("WGL_ARB_create_context")) {
   //                              int attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB, 3, WGL_CONTEXT_MINOR_VERSION_ARB, 3, WGL_CONTEXT_FLAGS_ARB,
   //                                    WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, 0};
   //                              hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
   //                              wglMakeCurrent(NULL, NULL);
   //                              wglDeleteContext(tempContext);
   //                              wglMakeCurrent(hdc, hglrc);
   //
   //                              //      int OpenGLVersion[2];
   //                              //      glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
   //                              //      glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
   //                      } else {
      hglrc = tempContext;
   //                      }

   //                      engine->resize(width, height);

   //wglSwapIntervalEXT(0);
   
   //SetWindowPos();
   ShowWindow(hwnd, SW_SHOW);
}

void Application::finalize() {
   wglMakeCurrent(hdc, NULL);
   wglDeleteContext(hglrc);
   ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK Application::MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
   Application* app = (Application*) GetWindowLongPtr(hwnd, 0);
   
   switch(msg) {
   case WM_CLOSE:
      PostQuitMessage(0);
      return 0;
      
   case WM_KEYUP:
      app->keyUp(wParam);
      return 0;
      
   case WM_KEYDOWN:
      app->keyDown(wParam);
      return 0;

   case WM_SIZE:
      app->resize(LOWORD(lParam), HIWORD(lParam));
      return 0;
   }
   
   return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Application::processEvents() {
   MSG msg;

   if(PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE) != 0) {
      if(msg.message == WM_QUIT) {
         running = false;
         return;
      }

      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }   
}

#endif //WIN32

