/*
 * engine.cpp
 *
 *  Created on: 01/04/2013
 *      Author: marrony
 */

#include "engine.h"
#include "swap_chain.h"

#include <cstring>
#include <stdlib.h>

int get_argument(int argc, char* argv[], const char* arg_name) {
	for(int i = 0; i < argc; i++) {
		if(!strcmp(arg_name, argv[i]) && i+1 < argc)
			return (WindowID)atoi(argv[i+1]);
	}

	return -1;
}

int main(int argc, char* argv[]) {
	WindowID handle = (WindowID)get_argument(argc, argv, "--window-id");
	int width = get_argument(argc, argv, "--width");
	int height = get_argument(argc, argv, "--height");

	if(handle == WINDOW_NULL)
		return 1;

	SwapChain swap_chain = create_swap_chain(handle, width, height);

	while(true) {
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1., 1., -1., 1., 1., 20.);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		glVertex3f(-.75, -.75, -1.0);
		glColor3f(0, 1, 0);
		glVertex3f(.75, -.75, -1.0);
		glColor3f(0, 0, 1);
		glVertex3f(.75, .75, -1.0);
		glEnd();

		swap_swap_chain(swap_chain);
	}

	return 0;
}
