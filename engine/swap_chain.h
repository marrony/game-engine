/*
 * swap_chain.h
 *
 *  Created on: 02/04/2013
 *      Author: marrony
 */

#ifndef SWAP_CHAIN_H_
#define SWAP_CHAIN_H_

#ifdef WIN32
#else
#include <GL/gl.h>
#include <GL/glx.h>
#endif //WIN32

#include "engine.h"

struct SwapChain {
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
};

SwapChain swap_chain_create(WindowID handle, int width, int height);
void swap_chain_destroy(SwapChain& swap_chain);
void swap_chain_swap_buffers(SwapChain& swap_chain);
void swap_chain_process_events(SwapChain& swap_chain);
void swap_chain_resize(SwapChain& swap_chain, int width, int height);

#endif /* SWAP_CHAIN_H_ */
