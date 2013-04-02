/*
 * swap_chain.cpp
 *
 *  Created on: 02/04/2013
 *      Author: marrony
 */

#include "swap_chain.h"

#include <stdio.h>

#ifdef WIN32

#else

SwapChain create_swap_chain(WindowID handle) {
	SwapChain swap_chain;

	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

	swap_chain.window = handle;
	swap_chain.display = XOpenDisplay(":0.0");

	fprintf(stderr, "Window: %d\n", (int)swap_chain.window);
	fprintf(stderr, "Display: %p\n", swap_chain.display);

	swap_chain.vi = glXChooseVisual(swap_chain.display, 0, att);

	fprintf(stderr, "Visual Info: %p\n", swap_chain.vi);

	Window root = DefaultRootWindow(swap_chain.display) ;
	Colormap cmap = XCreateColormap(swap_chain.display, root, swap_chain.vi->visual, AllocNone);

	XSetWindowColormap(swap_chain.display, swap_chain.window, cmap);

	swap_chain.glc = glXCreateContext(swap_chain.display, swap_chain.vi, NULL, GL_TRUE);
	fprintf(stderr, "GL Context: %p\n", swap_chain.glc);

	//TODO create another child window
	//BadMatch is generated if drawable was not created with the same X screen and visual as ctx. It is also generated if drawable is None and ctx is not NULL.
	glXMakeCurrent(swap_chain.display, swap_chain.window, swap_chain.glc);

	fprintf(stderr, "GL Context created\n");
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
