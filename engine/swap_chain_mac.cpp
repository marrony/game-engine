/*
 * swap_chain_mac.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: mneris
 */

#include "swap_chain.h"

#if __APPLE__

extern "C" void swap_chain_create(SwapChain* swap_chain, WindowID parent, int width, int height);
extern "C" void swap_chain_destroy(SwapChain* swap_chain);
extern "C" void swap_chain_swap_buffers(SwapChain* swap_chain);
extern "C" void swap_chain_process_events(SwapChain* swap_chain);
extern "C" void swap_chain_resize(SwapChain* swap_chain, int width, int height);

void SwapChain::create(WindowID handle, int width, int height) {
	swap_chain_create(this, handle, width, height);
}

void SwapChain::destroy() {
	swap_chain_destroy(this);
}

void SwapChain::swap_buffers() {
	swap_chain_swap_buffers(this);
}

void SwapChain::process_events() {
	swap_chain_process_events(this);
}

void SwapChain::resize(int width, int height) {
	swap_chain_resize(this, width, height);
}

#endif //__APPLE__
