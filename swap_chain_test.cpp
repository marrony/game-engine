/*
 * swap_chain_test.cpp
 *
 *  Created on: Nov 20, 2013
 *      Author: mneris
 */

#include "swap_chain.h"

int main() {
	SwapChain swap_chain;

	swap_chain.create(0, 600, 600);

	int id = glCreateProgram();

	while(true) {
		swap_chain.process_events();

		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		swap_chain.swap_buffers();
	}

	return 0;
}


