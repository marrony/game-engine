#ifndef APP_H
#define APP_H

#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
//#include <GL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glx.h>
#endif

#include <stdlib.h>
#include <stdio.h>

enum AppKeyCode {
   KEY_A = 0x41,
   KEY_B = 0x42,
   KEY_C = 0x43,
   KEY_D = 0x44,
   KEY_E = 0x45,
   KEY_F = 0x46,
   KEY_G = 0x47,
   KEY_H = 0x48,
   KEY_I = 0x49,
   KEY_J = 0x4a,
   KEY_K = 0x4b,
   KEY_L = 0x4c,
   KEY_M = 0x4d,
   KEY_N = 0x4e,
   KEY_O = 0x4f,
   KEY_P = 0x50,
   KEY_Q = 0x51,
   KEY_R = 0x52,
   KEY_S = 0x53,
   KEY_T = 0x54,
   KEY_U = 0x55,
   KEY_V = 0x56,
   KEY_W = 0x57,
   KEY_X = 0x58,
   KEY_Y = 0x59,
   KEY_Z = 0x5a,
};

class Game {
public:
   virtual void initialize() = 0;
   virtual void finalize() = 0;
   virtual void doFrame() = 0;
};

class Application {
#ifdef WIN32
   HWND hwnd;
   HDC hdc;
   HGLRC hglrc;
   
   static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#else
   Display *display;
   Window window;
   GLXContext glc;
   Atom WM_DELETE_MESSAGE;
#endif

   bool running;
   const char* title;
   int width;
   int height;
   bool fullscreen;
   
   bool keysPressed[256];
   
   void resize(int width, int height);
   void keyUp(int key);
   void keyDown(int key);
   
   void initialize();
   void finalize();
   void processEvents();
   void swapBuffers();
public:
   Application(const char* title, int width, int height, bool fullscreen);

   int run(Game& game);
   
   bool isKeyPressed(AppKeyCode key) const;
};

#endif //APP_H
