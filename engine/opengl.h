/*
 * opengl.h
 *
 *  Created on: 25/04/2013
 *      Author: marrony
 */

#ifndef OPENGL_H_
#define OPENGL_H_

#ifdef WIN32
#include <GL/glew.h>
#include <windows.h>
#include <GL/gl.h>
//#include <GL/glext.h>
typedef HWND WindowID;
#elif ANDROID
#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

//#include <android/sensor.h>
//#include <android/log.h>
#include <android_native_app_glue.h>

typedef ANativeWindow* WindowID;
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
typedef Window WindowID;
#endif

const WindowID WINDOW_NULL = (WindowID)-1;

#endif /* OPENGL_H_ */
