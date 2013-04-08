#include "app.h"
#include "exec.h"

#include <memory.h>

#include "socket.h"
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

	Socket sock;
	while(!sock.connect("127.0.0.1", 9090)) {
		fprintf(stderr, "trying again\n");
		fflush(stderr);
	}

	char buffer[1024];
	snprintf(buffer, sizeof(buffer), "{\"window\": %d, \"width\": %d, \"height\": %d}", (int)window, width, height);
	sock.send(buffer, strlen(buffer));

	game.initialize();

	running = true;
	while (running) {
		processEvents();
		game.doFrame();
		swapBuffers();
	}

	snprintf(buffer, sizeof(buffer), "{\"type\": \"finish\"}");
	sock.send(buffer, strlen(buffer));

	fprintf(stderr, "waiting a response\n");
	fflush(stderr);

	int nbytes = sock.recv(buffer, sizeof(buffer));
	if(nbytes > 0) {
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
