#include "app.h"

#include <memory.h>

Application::Application(const char* title, int width, int height, bool fullscreen) :
	running(false), title(title), width(width), height(height), fullscreen(fullscreen) {
	memset(keysPressed, 0, sizeof(keysPressed));
}

int Application::run(Game& game) {
	initialize();
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

		glViewport(0, 0, width, height);
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
