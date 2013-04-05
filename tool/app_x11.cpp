#include "app.h"

#ifndef WIN32

void Application::initializeVariables() {
	display = 0;
	window = 0;
	WM_DELETE_MESSAGE = 0;
}

void Application::swapBuffers() {
//	glXSwapBuffers(display, window);
}

void Application::initialize() {
	display = XOpenDisplay(NULL);

	if (display == NULL) {
		exit(1);
	}

	Window root = DefaultRootWindow(display) ;
	int screen = DefaultScreen(display);

	window = XCreateSimpleWindow(display, root, 0, 0, width, height, 1, BlackPixel(display, screen), WhitePixel(display, screen));
	XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask);

	XMapWindow(display, window);
	XStoreName(display, window, title);

	WM_DELETE_MESSAGE = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, window, &WM_DELETE_MESSAGE, 1);
}

void Application::finalize() {
	XDestroyWindow(display, window);
}

static AppKeyCode getAppKeyCode(KeySym keysym) {
	if (keysym >= XK_A && keysym <= XK_Z)
		return (AppKeyCode) (KEY_A + (keysym - XK_A));

	if (keysym >= XK_a && keysym <= XK_z)
		return (AppKeyCode) (KEY_A + (keysym - XK_a));

	if (keysym >= XK_0 && keysym <= XK_9)
		return (AppKeyCode) (KEY_0 + (keysym - XK_0));

	if (keysym >= XK_KP_0 && keysym <= XK_KP_0)
		return (AppKeyCode) (KEY_NUMPAD_0 + (keysym - XK_KP_0));

	if (keysym >= XK_F1 && keysym <= XK_F12)
		return (AppKeyCode) (KEY_F1 + (keysym - XK_F1));

	switch (keysym) {
	//case VK_LBUTTON: return KEY_LBUTTON;
	//case VK_RBUTTON: return KEY_RBUTTON;
	//case VK_MBUTTON: return KEY_MBUTTON
	//case VK_XBUTTON1: return KEY_XBUTTON1;
	//case VK_XBUTTON2: return KEY_XBUTTON2;
	case XK_BackSpace: return KEY_BACKSPACE;
	case XK_Tab: return KEY_TAB;
	case XK_Return: return KEY_ENTER;
	case XK_Escape: return KEY_ESCAPE;
	case XK_space: return KEY_SPACE;
	case XK_Page_Up: return KEY_PAGE_UP;
	case XK_Page_Down: return KEY_PAGE_DOWN;
	case XK_End: return KEY_END;
	case XK_Home: return KEY_HOME;
	case XK_Insert: return KEY_INSERT;
	case XK_Delete: return KEY_DELETE;
	case XK_Shift_L: return KEY_LEFT_SHIFT;
	case XK_Alt_L: return KEY_LEFT_ALT;
	case XK_Control_L: return KEY_LEFT_CONTROL;
	//case VK_LWIN: return KEY_LEFT_WIN;
	case XK_Shift_R: return KEY_RIGHT_SHIFT;
	case XK_Alt_R: return KEY_RIGHT_ALT;
	case XK_Control_R: return KEY_RIGHT_CONTROL;
	//case VK_RWIN: return KEY_RIGHT_WIN;
	case XK_Caps_Lock: return KEY_CAPS_LOCK;
	case XK_Left: return KEY_LEFT;
	case XK_Up: return KEY_UP;
	case XK_Right: return KEY_RIGHT;
	case XK_Down: return KEY_DOWN;
	case XK_Print: return KEY_PRINT_SCREEN;
	case XK_Scroll_Lock: return KEY_SCROLL_LOCK;
	case XK_Pause: return KEY_PAUSE;
	}

	return KEY_NONE;
}

void Application::processEvents() {
	XEvent event;
	XNextEvent(display, &event);

	switch (event.type) {
	case ClientMessage:
		if (event.xclient.data.l[0] == WM_DELETE_MESSAGE)
			running = false;
		break;

	case KeyRelease:
		onKeyUp(getAppKeyCode(XLookupKeysym(&event.xkey, 0)));
		break;

	case KeyPress:
		onKeyDown(getAppKeyCode(XLookupKeysym(&event.xkey, 0)));
		break;

	case ConfigureNotify:
		onResize(event.xconfigure.width, event.xconfigure.height);
		break;
	}

	if (event.type == Expose) {
		//XWindowAttributes gwa;
		//XGetWindowAttributes(display, window, &gwa);
		//glViewport(0, 0, gwa.width, gwa.height);
	}
}

#endif //WIN32