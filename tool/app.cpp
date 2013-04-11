#include "app.h"
#include "exec.h"
#include "json.h"
#include "protocol.h"

#include <memory.h>

Application::Application(const char* title, int width, int height, bool fullscreen) :
	running(false), title(title), width(width), height(height), fullscreen(fullscreen) {
	memset(keys_pressed, 0, sizeof(keys_pressed));
	initialize_variables();
}

int Application::run() {
	initialize();

//	const char* const args[] = {
//			"build/engine/engine",
//			"--port", "9090",
//			NULL
//	};
//
//	execute_program(args, NULL, NULL);

	while(!sock.connect("127.0.0.1", 9090)) {
		fprintf(stderr, "trying again\n");
		fflush(stderr);
	}

	protocol_send_window_message(sock, (intptr_t)window, width, height);
	protocol_send_load_mesh_message(sock, "quad.mesh");

	running = true;
	while (running) {
		process_events();
	}

	protocol_send_finish_message(sock);

	fprintf(stderr, "waiting a response\n");
	fflush(stderr);

	Json json;
	if(protocol_recv_message(sock, json)) {
		Value* type = json_get_attribute(json.value, "type");

		if(type && !strcmp("finish", type->string)) {
			fprintf(stderr, "close ok\n");
			fflush(stderr);
		}

		json_free(json);
	}

	sock.close();

	finalize();

	return 0;
}

void Application::stop_main_loop() {
	running = false;
}

void Application::on_resize(int width, int height) {
	if (this->width != width || this->height != height) {
		this->width = width;
		this->height = height;

		protocol_send_resize_message(sock, width, height);
	}
}

void Application::on_key_up(AppKeyCode key) {
	if (key == KEY_NONE)
		return;

	keys_pressed[key] = false;
}

void Application::on_key_down(AppKeyCode key) {
	if (key == KEY_NONE)
		return;

	keys_pressed[key] = true;
}

bool Application::is_key_pressed(AppKeyCode key) const {
	if (key == KEY_NONE)
		return false;

	return keys_pressed[key];
}
