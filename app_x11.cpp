#include "app.h"

#ifndef WIN32

void Application::swapBuffers() {
   glXSwapBuffers(display, window);
}

void Application::initialize() {
   Window root;
   Colormap cmap;
   XSetWindowAttributes swa;
   XVisualInfo *vi;
   GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
 
   display = XOpenDisplay(NULL);

   if (display == NULL) {
      //fprintf(stderr, "Cannot open display\n");
      exit(1);
   }
   
   root = DefaultRootWindow(display);

   vi = glXChooseVisual(display, 0, att);

   cmap = XCreateColormap(display, root, vi->visual, AllocNone);
   swa.colormap = cmap;
   swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask;

   window = XCreateWindow(display, root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

   XMapWindow(display, window);
   XStoreName(display, window, title);

   glc = glXCreateContext(display, vi, NULL, GL_TRUE);
   glXMakeCurrent(display, window, glc);
   //XSelectInput(display, window, ExposureMask | KeyPressMask);
 
   WM_DELETE_MESSAGE = XInternAtom(display, "WM_DELETE_WINDOW", False);
   XSetWMProtocols(display, window, &WM_DELETE_MESSAGE, 1);
}

void Application::finalize() {
   glXMakeCurrent(display, None, NULL);
   glXDestroyContext(display, glc);
   XDestroyWindow(display, window);
   XCloseDisplay(display);
}

void Application::processEvents() {
   XEvent event;
   XNextEvent(display, &event);
   
   switch(event.type) {
   case ClientMessage:
      if (event.xclient.data.l[0] == WM_DELETE_MESSAGE) {
         running = false;
      }
      break;
      
   case KeyRelease:
      //keyUp(XLookupKeysym(&event.xkey, 0));
      break;
      
   case KeyPress:
      //keyDown(XLookupKeysym(&event.xkey, 0));
      break;
      
   case ConfigureNotify:
      resize(event.xconfigure.width, event.xconfigure.height);
      break;
   }

   if (event.type == Expose) {
      //XWindowAttributes gwa;
      //XGetWindowAttributes(display, window, &gwa);
      //glViewport(0, 0, gwa.width, gwa.height);
   }
}

#endif //WIN32

