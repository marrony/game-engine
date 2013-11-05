/*
 * swap_chain_win32.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: mneris
 */

#include "swap_chain.h"

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

#endif //WIN32
