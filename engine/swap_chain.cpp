/*
 * swap_chain.cpp
 *
 *  Created on: 02/04/2013
 *      Author: marrony
 */

#include "swap_chain.h"

#include <stdio.h>

#ifdef WIN32

SwapChain create_swap_chain(WindowID handle, int width, int height) {
	SwapChain swap_chain;

	swap_chain.window = handle;

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

	swap_chain.hdc = GetWindowDC(swap_chain.window);

	int nPixelFormat = ChoosePixelFormat(swap_chain.hdc, &pfd);
	BOOL bResult = SetPixelFormat(swap_chain.hdc, nPixelFormat, &pfd);

	swap_chain.hglrc = wglCreateContext(swap_chain.hdc);
	wglMakeCurrent(swap_chain.hdc, swap_chain.hglrc);

	return swap_chain;
}

void destroy_swap_chain(SwapChain& swap_chain) {
	wglMakeCurrent(swap_chain.hdc, NULL);
	wglDeleteContext(swap_chain.hglrc);
	ReleaseDC(swap_chain.window, swap_chain.hdc);
}

void swap_swap_chain(SwapChain& swap_chain) {
	SwapBuffers(swap_chain.hdc);
}

#else

SwapChain create_swap_chain(WindowID handle, int width, int height) {
	SwapChain swap_chain;

	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

	swap_chain.parent = handle;
	swap_chain.display = XOpenDisplay(":0.0");

	Window root = DefaultRootWindow(swap_chain.display);

	swap_chain.vi = glXChooseVisual(swap_chain.display, 0, att);

	Colormap cmap = XCreateColormap(swap_chain.display, root, swap_chain.vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask;

	swap_chain.window = XCreateWindow(swap_chain.display, swap_chain.parent, 0, 0, width, height, 0, swap_chain.vi->depth, InputOutput, swap_chain.vi->visual, CWColormap | CWEventMask, &swa);

	XMapWindow(swap_chain.display, swap_chain.window);

	swap_chain.glc = glXCreateContext(swap_chain.display, swap_chain.vi, NULL, GL_TRUE);

	glXMakeCurrent(swap_chain.display, swap_chain.window, swap_chain.glc);

	return swap_chain;
}

void destroy_swap_chain(SwapChain& swap_chain) {
	glXMakeCurrent(swap_chain.display, None, NULL);
	glXDestroyContext(swap_chain.display, swap_chain.glc);
	XCloseDisplay(swap_chain.display);
}

void swap_swap_chain(SwapChain& swap_chain) {
	glXSwapBuffers(swap_chain.display, swap_chain.window);
}

#endif //WIN32
