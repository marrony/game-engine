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
   
   bool isKeyPressed(int key) const;
};

#endif //APP_H
