/*
 * opengl.h
 *
 *  Created on: 25/04/2013
 *      Author: marrony
 */

#ifndef OPENGL_H_
#define OPENGL_H_

#include <GL/glew.h>

#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
//#include <GL/glext.h>
typedef HWND WindowID;
#else
#include <GL/gl.h>
#include <GL/glx.h>
typedef Window WindowID;
#endif

const WindowID WINDOW_NULL = (WindowID)-1;

#endif /* OPENGL_H_ */
