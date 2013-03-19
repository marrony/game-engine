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
   KEY_NONE = -1,
   KEY_LBUTTON = 0,
   KEY_RBUTTON,
   KEY_MBUTTON,
   KEY_XBUTTON1,
   KEY_XBUTTON2,
   
   KEY_BACKSPACE = 0x08,
   KEY_TAB = 0x09,
   
   KEY_ENTER = 0x0d,
   
   KEY_ESCAPE = 0x1b,
   KEY_SPACE = 0x20,
   KEY_PAGE_UP = 0x21,
   KEY_PAGE_DOWN = 0x22,
   KEY_END = 0x23,
   KEY_HOME = 0x24,
   KEY_INSERT = 0x25,
   KEY_DELETE = 0x26,
   
   KEY_0 = 0x30,
   KEY_1 = 0x31,
   KEY_2 = 0x32,
   KEY_3 = 0x33,
   KEY_4 = 0x34,
   KEY_5 = 0x35,
   KEY_6 = 0x36,
   KEY_7 = 0x37,
   KEY_8 = 0x38,
   KEY_9 = 0x39,
   
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
   
   KEY_NUMPAD_0 = 0x60,
   KEY_NUMPAD_1 = 0x61,
   KEY_NUMPAD_2 = 0x62,
   KEY_NUMPAD_3 = 0x63,
   KEY_NUMPAD_4 = 0x64,
   KEY_NUMPAD_5 = 0x65,
   KEY_NUMPAD_6 = 0x66,
   KEY_NUMPAD_7 = 0x67,
   KEY_NUMPAD_8 = 0x68,
   KEY_NUMPAD_9 = 0x69,
   KEY_MULTIPLY = 0x6a,
   KEY_ADD = 0x6b,
   KEY_COMMA = 0x6c,
   KEY_SUBTRACT = 0x6d,
   KEY_DECIMAL = 0x6e,
   KEY_DIVIDE = 0x6f,
   
   KEY_F1 = 0x70,
   KEY_F2 = 0x71,
   KEY_F3 = 0x72,
   KEY_F4 = 0x73,
   KEY_F5 = 0x74,
   KEY_F6 = 0x75,
   KEY_F7 = 0x76,
   KEY_F8 = 0x77,
   KEY_F9 = 0x78,
   KEY_F10 = 0x79,
   KEY_F11 = 0x7a,
   KEY_F12 = 0x7b,
   
   KEY_LEFT_SHIFT = 0x90,
   KEY_LEFT_ALT = 0x91,
   KEY_LEFT_CONTROL = 0x92,
   KEY_LEFT_WIN = 0x93,
   
   KEY_RIGHT_SHIFT = 0x94,
   KEY_RIGHT_ALT = 0x95,
   KEY_RIGHT_CONTROL = 0x96,
   KEY_RIGHT_WIN = 0x97,
   
   KEY_CAPS_LOCK,
   KEY_LEFT,
   KEY_UP,
   KEY_RIGHT,
   KEY_DOWN,
   KEY_PRINT_SCREEN,
   KEY_SCROLL_LOCK,
   KEY_PAUSE,
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
   
   void onResize(int width, int height);
   void onKeyUp(AppKeyCode key);
   void onKeyDown(AppKeyCode key);
   
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
