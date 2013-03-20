#include "app.h"

class Game2 : public Game {
   Application& app;
public:
   Game2(Application& app) : app(app) { }

   virtual void initialize() {
   }
   
   virtual void finalize() {
   }

   virtual void doFrame() {
      if(app.isKeyPressed(KEY_ESCAPE)) {
         app.stopMainLoop();
         return;
      }
   
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

struct Method {
   void invoke() {
      fprintf(stderr, "invoke\n");
   }
};

template<typename M, typename T>
Method createMethod(M m, T* t) {
   (t->*m)();
}

struct A {
   void teste() {
      fprintf(stderr, "teste\n");
   }
};

int main(void) {
   A a;
   Method m = createMethod(&A::teste, &a);
   m.invoke();

   return 0;
   Application app("my x11/win32 window", 600, 600, true);
   Game2 game(app);
   return app.run(game);
}

