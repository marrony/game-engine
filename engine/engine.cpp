/*
 * engine.cpp
 *
 *  Created on: 01/04/2013
 *      Author: marrony
 */

#include <cstring>

#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
typedef HWND WindowID;
#else
#include <GL/gl.h>
#include <GL/glx.h>
typedef Window WindowID;
#endif

const WindowID WINDOW_NULL = (WindowID)-1;

WindowID get_window_handle(int argc, char* argv[]) {
	for(int i = 0; i < argc; i++) {
		if(!strcmp("--window-id", argv[i]) && i+1 < argc)
			return (WindowID)atoi(argv[i+1]);
	}

	return (WindowID)-1;
}

int main(int argc, char* argv[]) {
	WindowID handle = get_window_handle(argc, argv);

	if(handle == WINDOW_NULL)
		return 1;

	return 0;
}
