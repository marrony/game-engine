#ifndef WIN32
#include <GL/gl.h>
#include <GL/glx.h>
#else
#include <windows.h>
#include <GL/gl.h>
//#include <GL/glext.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Game {
public:
   virtual void initialize() = 0;
   virtual void finalize() = 0;
   virtual void doFrame() = 0;
};

class Application {
#ifndef WIN32
   Display *d;
   Window w;
   GLXContext glc;
   Atom WM_DELETE_MESSAGE;
#else
   HWND hwnd;
   HDC hdc;
   HGLRC hglrc;
#endif

   bool running;
   
   void initialize();
   void finalize();
   void processEvents();
public:
   void run(Game& game);
};

void Application::run(Game& game) {
   initialize();
   game.initialize();

   running = true;
   while(running) {
      processEvents();
      game.doFrame();
#ifndef WIN32
      glXSwapBuffers(d, w);
#else
      wglSwapLayerBuffers(hdc, WGL_SWAP_MAIN_PLANE);
#endif
   }
   
   game.finalize();
   finalize();
}

#ifdef WIN32
static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
   Application* app = (Application*) GetWindowLongPtr(hwnd, 0);
   
   switch(msg) {
   case WM_CLOSE:
      PostQuitMessage(0);
      break;
   
   case WM_SIZE:
      glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
      break;
   }
   
   //return app->wndProc(msg, wParam, lParam);
   return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif

void Application::initialize() {
#ifndef WIN32
   Window root;
   Colormap cmap;
   XSetWindowAttributes swa;
   XVisualInfo *vi;
   GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
 
   d = XOpenDisplay(NULL);

   if (d == NULL) {
      fprintf(stderr, "Cannot open display\n");
      exit(1);
   }
   
   root = DefaultRootWindow(d);

   vi = glXChooseVisual(d, 0, att);

   cmap = XCreateColormap(d, root, vi->visual, AllocNone);
   swa.colormap = cmap;
   swa.event_mask = ExposureMask | KeyPressMask;

   w = XCreateWindow(d, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

   XMapWindow(d, w);
   XStoreName(d, w, "VERY SIMPLE APPLICATION");

   glc = glXCreateContext(d, vi, NULL, GL_TRUE);
   glXMakeCurrent(d, w, glc);
   //XSelectInput(d, w, ExposureMask | KeyPressMask);
 
   WM_DELETE_MESSAGE = XInternAtom(d, "WM_DELETE_WINDOW", False);
   XSetWMProtocols(d, w, &WM_DELETE_MESSAGE, 1);
#else
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
         "",
         WS_OVERLAPPEDWINDOW,
         CW_USEDEFAULT,
         CW_USEDEFAULT,
         CW_USEDEFAULT,
         CW_USEDEFAULT,
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
   
   ShowWindow(hwnd, SW_SHOW);
#endif
}

void Application::finalize() {
#ifndef WIN32
   glXMakeCurrent(d, None, NULL);
   glXDestroyContext(d, glc);
   XDestroyWindow(d, w);
   XCloseDisplay(d);
#else
   wglMakeCurrent(hdc, NULL);
   wglDeleteContext(hglrc);
   ReleaseDC(hwnd, hdc);
#endif
}

void Application::processEvents() {
#ifndef WIN32
   XEvent e;
   XNextEvent(d, &e);

   if (e.type == Expose) {
      XWindowAttributes gwa;
      XGetWindowAttributes(d, w, &gwa);
      glViewport(0, 0, gwa.width, gwa.height);
   }

   if (e.type == KeyPress) {
      running = false;
   }

   if (e.type == ClientMessage && e.xclient.data.l[0] == WM_DELETE_MESSAGE) {
      running = false;
   }
#else
   MSG msg;

   if(PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE) != 0) {
      if(msg.message == WM_QUIT) {
         running = false;
         return;
      }

      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }   
#endif
}

class Game2 : public Game {
   Application& app;
public:
   Game2(Application& app) : app(app) { }

   virtual void initialize() {
   }
   
   virtual void finalize() {
   }

   virtual void doFrame() {
      glClearColor(1.0, 1.0, 1.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(-1., 1., -1., 1., 1., 20.);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      //gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

      glBegin(GL_TRIANGLES);
      glColor3f(1, 0, 0); glVertex3f(-.75, -.75, -1.0);
      glColor3f(0, 1, 0); glVertex3f( .75, -.75, -1.0);
      glColor3f(0, 0, 1); glVertex3f( .75,  .75, -1.0);
      glEnd();
   }
};

int main(void) {
   Application app;
   Game2 game(app);
   app.run(game);

   return 0;
}

