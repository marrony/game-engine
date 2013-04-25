/*
 * swap_chain.h
 *
 *  Created on: 02/04/2013
 *      Author: marrony
 */

#ifndef SWAP_CHAIN_H_
#define SWAP_CHAIN_H_

#include "opengl.h"

class SwapChain {
#ifdef WIN32
	HWND parent;
	HWND window;
	HDC hdc;
	HGLRC hglrc;
#else
	Window parent;
	Window window;
	Display* display;
	GLXContext glc;
	XVisualInfo *vi;
#endif

public:
	void create(WindowID handle, int width, int height);
	void destroy();
	void swap_buffers();
	void process_events();
	void resize(int width, int height);
};

#endif /* SWAP_CHAIN_H_ */
