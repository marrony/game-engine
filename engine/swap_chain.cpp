/*
 * swap_chain.cpp
 *
 *  Created on: 02/04/2013
 *      Author: marrony
 */

#include "swap_chain.h"

#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
//#include <GL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glx.h>
#endif

#include <stdio.h>

#ifdef WIN32

SwapChain swap_chain_create(WindowID handle, int width, int height) {
	SwapChain swap_chain;

	swap_chain.parent = handle;

	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcx.lpfnWndProc = DefWindowProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = GetModuleHandle(NULL);
	wcx.hIcon = NULL;
	wcx.hCursor = NULL;
	wcx.hbrBackground = NULL;
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = "SwapChainClass";
	wcx.hIconSm = NULL;

	RegisterClassEx(&wcx);

	swap_chain.window = CreateWindowEx(
			WS_EX_NOPARENTNOTIFY,
			"SwapChainClass",
			NULL,
			WS_CLIPSIBLINGS | WS_CHILD,
			0,
			0,
			width,
			height,
			swap_chain.parent,
			(HMENU) NULL,
			GetModuleHandle(NULL),
			(LPVOID)NULL);

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

	swap_chain.hdc = GetDC(swap_chain.window);

	int nPixelFormat = ChoosePixelFormat(swap_chain.hdc, &pfd);
	BOOL bResult = SetPixelFormat(swap_chain.hdc, nPixelFormat, &pfd);

	swap_chain.hglrc = wglCreateContext(swap_chain.hdc);
	wglMakeCurrent(swap_chain.hdc, swap_chain.hglrc);

	ShowWindow(swap_chain.window, SW_SHOW);

	return swap_chain;
}

void swap_chain_destroy(SwapChain& swap_chain) {
	wglMakeCurrent(swap_chain.hdc, NULL);
	wglDeleteContext(swap_chain.hglrc);
	ReleaseDC(swap_chain.window, swap_chain.hdc);
	DestroyWindow(swap_chain.window);
}

void swap_chain_swap_buffers(SwapChain& swap_chain) {
	SwapBuffers(swap_chain.hdc);
}

void swap_chain_process_events(SwapChain& swap_chain) {
	MSG msg;

	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
		if(msg.message == WM_QUIT) {
			return;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void swap_chain_resize(SwapChain& swap_chain, int width, int height) {
	RECT pr;
	GetClientRect(swap_chain.parent, &pr);
	SetWindowPos(swap_chain.window, NULL, 0, 0, pr.right - pr.left, pr.bottom - pr.top, SWP_DEFERERASE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOMOVE);
}

#else

SwapChain swap_chain_create(WindowID handle, int width, int height) {
	SwapChain swap_chain;

	XInitThreads();

	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

	swap_chain.parent = handle;
	swap_chain.display = XOpenDisplay(":0.0");

	Window root = DefaultRootWindow(swap_chain.display);

	swap_chain.vi = glXChooseVisual(swap_chain.display, 0, att);

	Colormap cmap = XCreateColormap(swap_chain.display, root, swap_chain.vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask;// | KeyPressMask | KeyReleaseMask | StructureNotifyMask;

	swap_chain.window = XCreateWindow(swap_chain.display, swap_chain.parent, 0, 0, width, height, 0, swap_chain.vi->depth, InputOutput, swap_chain.vi->visual, CWColormap | CWEventMask, &swa);

	XMapWindow(swap_chain.display, swap_chain.window);

	swap_chain.glc = glXCreateContext(swap_chain.display, swap_chain.vi, NULL, GL_TRUE);

	glXMakeCurrent(swap_chain.display, swap_chain.window, swap_chain.glc);

	return swap_chain;
}

void swap_chain_destroy(SwapChain& swap_chain) {
	//XLockDisplay(swap_chain.display);
	glXMakeCurrent(swap_chain.display, None, NULL);
	glXDestroyContext(swap_chain.display, swap_chain.glc);
	XCloseDisplay(swap_chain.display);
	//XUnlockDisplay(swap_chain.display);
}

void swap_chain_swap_buffers(SwapChain& swap_chain) {
	//XLockDisplay(swap_chain.display);
	glXSwapBuffers(swap_chain.display, swap_chain.window);
	//XUnlockDisplay(swap_chain.display);
}

void swap_chain_process_events(SwapChain& swap_chain) {
	//XLockDisplay(swap_chain.display);
	XEvent event;
	while(XQLength(swap_chain.display) > 0)
		XNextEvent(swap_chain.display, &event);
	//XUnlockDisplay(swap_chain.display);
}

void swap_chain_resize(SwapChain& swap_chain, int width, int height) {
	//XLockDisplay(swap_chain.display);
	XMoveResizeWindow(swap_chain.display, swap_chain.window, 0, 0, width, height);
	//XResizeWindow(swap_chain.display, swap_chain.window, width, height);
	//XUnlockDisplay(swap_chain.display);
}

#endif //WIN32
