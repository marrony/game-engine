#include "app.h"
#include "exec.h"

#include <memory.h>

#include "json.h"

Application::Application(const char* title, int width, int height, bool fullscreen) :
	running(false), title(title), width(width), height(height), fullscreen(fullscreen) {
	memset(keysPressed, 0, sizeof(keysPressed));
	initializeVariables();
}

int Application::run(Game& game) {
	initialize();

	const char* const args[] = {
			"build/engine/engine",
			"--port", "9090",
			NULL
	};

	execute_program(args, NULL, NULL);

	while(!sock.connect("127.0.0.1", 9090)) {
		fprintf(stderr, "trying again\n");
		fflush(stderr);
	}

	char buffer[1024];
	size_t nbytes;
	uint32_t size;

	snprintf(buffer, sizeof(buffer), "{\"window\": %d, \"width\": %d, \"height\": %d}", (int)window, width, height);
	size = strlen(buffer);

	sock.send(&size, sizeof(size), nbytes);
	sock.send(buffer, size, nbytes);

	game.initialize();

	running = true;
	while (running) {
		processEvents();
		game.doFrame();
		swapBuffers();
	}

	snprintf(buffer, sizeof(buffer), "{\"type\": \"finish\"}");
	size = strlen(buffer);

	sock.send(&size, sizeof(size), nbytes);
	sock.send(buffer, size, nbytes);

	fprintf(stderr, "waiting a response\n");
	fflush(stderr);

	if(sock.recv(&size, sizeof(size), nbytes) >= 0) {
		sock.recv(buffer, size, nbytes);
		buffer[nbytes] = 0;

		Json json = json_parse(buffer, nbytes);

		Value* type = json_get_attribute(json.value, "type");
		if(type && !strcmp("finish", type->string)) {
			fprintf(stderr, "close ok\n");
			fflush(stderr);
		}

		json_free(json);
	}

	game.finalize();

	sock.close();

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

		char buffer[256];
		size_t nbytes;
		uint32_t size;

		snprintf(buffer, sizeof(buffer), "{\"type\": \"resize\", \"width\": %d, \"height\": %d}", width, height);
		size = strlen(buffer);

		sock.send(&size, sizeof(size), nbytes);
		sock.send(buffer, size, nbytes);
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
