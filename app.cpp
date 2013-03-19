#include "app.h"

Application::Application(const char* title, int width, int height, bool fullscreen) :
   title(title), width(width), height(height), fullscreen(fullscreen) {
}

int Application::run(Game& game) {
   initialize();
   game.initialize();

   running = true;
   while(running) {
      processEvents();
      game.doFrame();
      swapBuffers();
   }
   
   game.finalize();
   finalize();
   
   return 0;
}

void Application::resize(int width, int height) {
   if(this->width != width && this->height != height) {
      this->width = width;
      this->height = height;
      
      glViewport(0, 0, width, height);
   }
}

void Application::keyUp(AppKeyCode key) {
   keysPressed[key] = false;
}

void Application::keyDown(AppKeyCode key) {
   keysPressed[key] = true;
}

bool Application::isKeyPressed(AppKeyCode key) const {
   return keysPressed[key];
}
