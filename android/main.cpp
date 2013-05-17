/*
 * main.cpp
 *
 *  Created on: 16/05/2013
 *      Author: marrony
 */

#include "engine.h"
#include "opengl.h"

void android_main(struct android_app* state) {
	app_dummy();

	Engine engine;

	state->userData = &engine;
	//state->onAppCmd = engine_handle_cmd;
	//state->onInputEvent = engine_handle_input;

	engine.initialize(state->window, 0, 0);

	engine.load_mesh("/storage/sdcard0/teste.mesh");

	while (true) {
		// Read all pending events.
		int ident;
		int events;
		struct android_poll_source* source;

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((ident = ALooper_pollAll(0, NULL, &events, (void**) &source)) >= 0) {

			// Process this event.
			if (source != NULL) {
				source->process(state, source);
			}

//			// If a sensor has data, process it now.
//			if (ident == LOOPER_ID_USER) {
//				if (engine.accelerometerSensor != NULL) {
//					ASensorEvent event;
//					while (ASensorEventQueue_getEvents(engine.sensorEventQueue, &event, 1) > 0) {
//						LOGI("accelerometer: x=%f y=%f z=%f",
//								event.acceleration.x, event.acceleration.y,
//								event.acceleration.z);
//					}
//				}
//			}

			// Check if we are exiting.
			if (state->destroyRequested != 0)
				break;
		}

		engine.runOneFrame();
	}

	engine.finalize();
}
