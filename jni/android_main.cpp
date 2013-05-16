/*
 * android_main.cpp
 *
 *  Created on: 15/05/2013
 *      Author: marrony
 */
#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
//#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

void android_main(struct android_app* state) {
	// Make sure glue isn't stripped.
	app_dummy();
}
