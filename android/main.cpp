/*
 * main.cpp
 *
 *  Created on: 16/05/2013
 *      Author: marrony
 */

#include "engine.h"
#include "opengl.h"

void android_main(struct android_app* state) {
	//app_dummy();

	Engine engine;

	engine.initialize(9090);
	engine.run();
	engine.finalize();
}
