/*
 * swap_chain_android.cpp
 *
 *  Created on: 02/04/2013
 *      Author: marrony
 */

#include "swap_chain.h"

#if ANDROID

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

void SwapChain::create(WindowID handle, int width, int height) {
	/*
	 * Here specify the attributes of the desired configuration.
	 * Below, we select an EGLConfig with at least 8 bits per color
	 * component compatible with on-screen windows
	 */
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};

	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Here, the application chooses the configuration it desires. In this
	 * sample, we have a very simplified selection process, where we pick
	 * the first EGLConfig that matches our criteria */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	 * As soon as we picked a EGLConfig, we can safely reconfigure the
	 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(handle, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, handle, NULL);
	const EGLint attrib_list[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
	context = eglCreateContext(display, config, NULL, attrib_list);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		switch(glGetError()) {
		case EGL_BAD_DISPLAY:
			LOGW("EGL_BAD_DISPLAY"); break;
		case EGL_NOT_INITIALIZED:
			LOGW("EGL_NOT_INITIALIZED"); break;
		case EGL_BAD_SURFACE:
			LOGW("EGL_BAD_SURFACE"); break;
		case EGL_BAD_CONTEXT:
			LOGW("EGL_BAD_CONTEXT"); break;
		case EGL_BAD_MATCH:
			LOGW("EGL_BAD_MATCH"); break;
		case EGL_BAD_ACCESS:
			LOGW("EGL_BAD_ACCESS"); break;
		case EGL_BAD_NATIVE_PIXMAP:
			LOGW("EGL_BAD_NATIVE_PIXMAP"); break;
		case EGL_BAD_NATIVE_WINDOW:
			LOGW("EGL_BAD_NATIVE_WINDOW"); break;
		case EGL_BAD_CURRENT_SURFACE:
			LOGW("EGL_BAD_CURRENT_SURFACE"); break;
		case EGL_BAD_ALLOC:
			LOGW("EGL_BAD_ALLOC"); break;
		case EGL_CONTEXT_LOST:
			LOGW("EGL_CONTEXT_LOST"); break;
		}
		//LOGW("Unable to eglMakeCurrent");
		return;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &this->width);
	eglQuerySurface(display, surface, EGL_HEIGHT, &this->height);
}

void SwapChain::destroy() {
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }

    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}

void SwapChain::swap_buffers() {
	eglSwapBuffers(display, surface);
}

void SwapChain::process_events() {
}

void SwapChain::resize(int width, int height) {
}

#endif //ANDROID
