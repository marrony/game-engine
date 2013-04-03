#include "app.h"
#include "exec.h"

#include <memory.h>

Application::Application(const char* title, int width, int height, bool fullscreen) :
	running(false), title(title), width(width), height(height), fullscreen(fullscreen) {
	memset(keysPressed, 0, sizeof(keysPressed));
	initializeVariables();
}

int Application::run(Game& game) {
	initialize();

	char window_id[32+1];
	char width_str[32+1];
	char height_str[32+1];

	snprintf(window_id, 32, "%d", (int)window);
	snprintf(width_str, 32, "%d", width);
	snprintf(height_str, 32, "%d", height);

	const char* const args[] = {
			"build/engine/engine",
			"--window-id", window_id,
			"--width", width_str,
			"--height", height_str,
			NULL
	};

	execute_program(args, NULL, NULL);

	game.initialize();

	running = true;
	while (running) {
		processEvents();
		game.doFrame();
		swapBuffers();
	}

	game.finalize();
	finalize();

	return 0;
}

void Application::stopMainLoop() {
	running = false;
}

void Application::onResize(int width, int height) {
	if (this->width != width || this->height != height) {
		this->width = width;
		this->height = height;

//		glViewport(0, 0, width, height);
	}
}

void Application::onKeyUp(AppKeyCode key) {
	if (key == KEY_NONE)
		return;

	keysPressed[key] = false;
}

void Application::onKeyDown(AppKeyCode key) {
	if (key == KEY_NONE)
		return;

	keysPressed[key] = true;
}

bool Application::isKeyPressed(AppKeyCode key) const {
	if (key == KEY_NONE)
		return false;

	return keysPressed[key];
}
