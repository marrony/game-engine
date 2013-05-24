#include "app.h"
#include "exec.h"
#include "json.h"
#include "json_io.h"
#include "protocol.h"

#include <memory.h>

Application::Application(const char* title, int width, int height, bool fullscreen) :
	running(false), title(title), width(width), height(height), fullscreen(fullscreen) {
	memset(keys_pressed, 0, sizeof(keys_pressed));
	initialize_variables();
}

#define STRINGFY(x) #x

void protocol_send_create_material_message(Socket sock) {
	char buffer[MAX_PROTOCOL_PACKET_SIZE];
	char data[MAX_PROTOCOL_PACKET_SIZE];

	const char* vertex_shader = STRINGFY(
		uniform mat4 modelViewMatrix;
		uniform mat4 projectionMatrix;
		uniform mat3 normalMatrix;

		attribute vec4 vPosition;
		attribute vec3 vNormal;

		varying vec3 N;
		varying vec4 V;

		void main() {
			N = normalMatrix * vNormal;
			V = modelViewMatrix * vPosition;
			gl_Position = projectionMatrix * modelViewMatrix * vPosition;
		}
	);

	const char* fragment_shader = STRINGFY(
		uniform vec3 lightPosition;

		varying vec3 N;
		varying vec4 V;

		void main() {
			vec3 light_dir = normalize(lightPosition - V.xyz);
			vec3 normal = normalize(N);

			float shade = clamp(dot(normal, light_dir), 0, 1);
			gl_FragColor = vec4(0.0, 1.0, 0.0, 0.5) * vec4(vec3(shade), 1);
		}
	);

	Json json = json_initialize(data, MAX_PROTOCOL_PACKET_SIZE);
	json.root = json_create_object(json);

	Value passes = json_create_array(json);

	json_set_attribute(json, json.root, "type", json_create_string(json, "create-material"));
	json_set_attribute(json, json.root, "fragment-shader", json_create_string(json, fragment_shader));
	json_set_attribute(json, json.root, "vertex-shader", json_create_string(json, vertex_shader));

	json_serialize(json, buffer, MAX_PROTOCOL_PACKET_SIZE);

	json_free(json);

	protocol_send_raw_packet(sock, buffer, strlen(buffer));
}

#undef STRINGFY

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
	protocol_send_create_material_message(sock);
	protocol_send_load_mesh_message(sock, "Seymour_triangulate.dae.mesh");
	protocol_send_load_mesh_message(sock, "duck_triangulate.dae.mesh");

	running = true;
	while (running) {
		process_events();
	}

	protocol_send_finish_message(sock);

	fprintf(stderr, "waiting a response\n");
	fflush(stderr);

	Json json;
	if(protocol_recv_message(sock, json)) {
		Value* type = json_get_attribute(json, json.root, "type");

		if(type && !strcmp("finish", json.data+type->string)) {
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

	if(key == KEY_F1)
		protocol_send_load_mesh_message(sock, "duck_triangulate.mesh");

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
