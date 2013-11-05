/*
 * swap_chain_x11.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: mneris
 */

#include "swap_chain.h"

#if __linux__

void SwapChain::create(WindowID handle, int width, int height) {
	SwapChain swap_chain;

	XInitThreads();

	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

	parent = handle;
	display = XOpenDisplay(":0.0");

	Window root = DefaultRootWindow(display);

	vi = glXChooseVisual(display, 0, att);

	Colormap cmap = XCreateColormap(display, root, vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask;// | KeyPressMask | KeyReleaseMask | StructureNotifyMask;

	window = XCreateWindow(display, parent, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

	XMapWindow(display, window);

	glc = glXCreateContext(display, vi, NULL, GL_TRUE);

	glXMakeCurrent(display, window, glc);

	glewInit();
}

void SwapChain::destroy() {
	//XLockDisplay(display);
	glXMakeCurrent(display, None, NULL);
	glXDestroyContext(display, glc);
	XCloseDisplay(display);
	//XUnlockDisplay(display);
}

void SwapChain::swap_buffers() {
	//XLockDisplay(display);
	glXSwapBuffers(display, window);
	//XUnlockDisplay(display);
}

void SwapChain::process_events() {
	//XLockDisplay(display);
	XEvent event;
	while(XQLength(display) > 0)
		XNextEvent(display, &event);
	//XUnlockDisplay(display);
}

void SwapChain::resize(int width, int height) {
	//XLockDisplay(display);
	XMoveResizeWindow(display, window, 0, 0, width, height);
	//XResizeWindow(display, window, width, height);
	//XUnlockDisplay(display);
}

#endif //__linux__
