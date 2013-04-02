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
	Window window;
	Display* display;
	GLXContext glc;
	XVisualInfo *vi;
};

SwapChain create_swap_chain(WindowID handle);
void destroy_swap_chain(SwapChain& swap_chain);
void swap_swap_chain(SwapChain& swap_chain);

#endif /* SWAP_CHAIN_H_ */
