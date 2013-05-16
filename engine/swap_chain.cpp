/*
 * cpp
 *
 *  Created on: 02/04/2013
 *      Author: marrony
 */

#include "swap_chain.h"

#include <stdio.h>

#ifdef WIN32

void SwapChain::create(WindowID handle, int width, int height) {
	parent = handle;

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

	window = CreateWindowEx(
			WS_EX_NOPARENTNOTIFY,
			"SwapChainClass",
			NULL,
			WS_CLIPSIBLINGS | WS_CHILD,
			0,
			0,
			width,
			height,
			parent,
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

	hdc = GetDC(window);

	int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	BOOL bResult = SetPixelFormat(hdc, nPixelFormat, &pfd);

	hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);

	glewInit();

	ShowWindow(window, SW_SHOW);
}

void SwapChain::destroy() {
	wglMakeCurrent(hdc, NULL);
	wglDeleteContext(hglrc);
	ReleaseDC(window, hdc);
	DestroyWindow(window);
}

void SwapChain::swap_buffers() {
	SwapBuffers(hdc);
}

void SwapChain::process_events() {
	MSG msg;

	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
		if(msg.message == WM_QUIT) {
			return;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void SwapChain::resize(int width, int height) {
	RECT pr;
	GetClientRect(parent, &pr);
	SetWindowPos(window, NULL, 0, 0, pr.right - pr.left, pr.bottom - pr.top, SWP_DEFERERASE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOMOVE);
}

#elif ANDROID
void SwapChain::create(WindowID handle, int width, int height) {
	/*
	 * Here specify the attributes of the desired configuration.
	 * Below, we select an EGLConfig with at least 8 bits per color
	 * component compatible with on-screen windows
	 */
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};

	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Here, the application chooses the configuration it desires. In this
	 * sample, we have a very simplified selection process, where we pick
	 * the first EGLConfig that matches our criteria */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	 * As soon as we picked a EGLConfig, we can safely reconfigure the
	 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(handle, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, handle, NULL);
	context = eglCreateContext(display, config, NULL, NULL);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		//LOGW("Unable to eglMakeCurrent");
		return;
	}

	//eglQuerySurface(display, surface, EGL_WIDTH, &w);
	//eglQuerySurface(display, surface, EGL_HEIGHT, &h);
}

void SwapChain::destroy() {
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }

    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}

void SwapChain::swap_buffers() {
	eglSwapBuffers(display, surface);
}

void SwapChain::process_events() {
}

void SwapChain::resize(int width, int height) {
}

#else

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

#endif //WIN32