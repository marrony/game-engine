/*
 * main.cpp
 *
 *  Created on: 16/05/2013
 *      Author: marrony
 */
#include "engine.h"

enum Endian {
	LittleEndian,
	BigEndian,
	NonIeee
};

Endian detect_endianess() {
	// detect computer architecture
	union {
		double f;
		uint32_t i[2];
	} convert;

	convert.f = 1.0;

	if (convert.i[1] == 0x3FF00000)
		return LittleEndian;
	else if (convert.i[0] == 0x3FF00000)
		return BigEndian;
	else
		return NonIeee;
}

int get_argument(int argc, char* argv[], const char* arg_name) {
	for(int i = 0; i < argc; i++) {
		if(!strcmp(arg_name, argv[i]) && i+1 < argc)
			return atoi(argv[i+1]);
	}

	return -1;
}

int main(int argc, char* argv[]) {
	detect_endianess();

	int port = get_argument(argc, argv, "--port");

	Engine engine;
	ServerSocket server;
	Socket client;

	server.create(port < 0 ? 9090 : port);

	fprintf(stderr, "waint for connection\n");
	fflush(stderr);

	client = server.accept();

	fprintf(stderr, "connected: %d\n", *(int*)&client);
	fflush(stderr);

	Json json;
	protocol_recv_message(client, json);

	Value* window_value = json_get_attribute(json, json.root, "window");
	Value* width_value = json_get_attribute(json, json.root, "width");
	Value* height_value = json_get_attribute(json, json.root, "height");

	int width = width_value->integer;
	int height = height_value->integer;
	engine.initialize((WindowID)(intptr_t)window_value->integer, width, height);

	json_free(json);

	while(true) {
		if(client.has_data()) {
			Json json;

			if(!protocol_recv_message(client, json))
				break;
			else {
				Value* type = json_get_attribute(json, json.root, "type");

				if(type) {
					if(!strcmp("finish", json.data+type->string)) {
						fprintf(stderr, "type: %s\n", json.data+type->string);
						fflush(stderr);

						break;
					} else if(!strcmp("resize", json.data+type->string)) {
						Value* width_value = json_get_attribute(json, json.root, "width");
						Value* height_value = json_get_attribute(json, json.root, "height");

						width = width_value->integer;
						height = height_value->integer;

						fprintf(stderr, "resize %dx%d\n", width, height);
						fflush(stderr);

						engine.resize(width, height);
						//swap_chain_resize(swap_chain, width, height);
					} else if(!strcmp("load-mesh", json.data+type->string)) {
						Value* mesh = json_get_attribute(json, json.root, "mesh");
						engine.load_mesh(json.data+mesh->string);
					}
				}

				json_free(json);
			}
		}

		engine.runOneFrame();
	}

	engine.finalize();

	protocol_send_finish_message(client);

	client.close();
	server.close();

	return 0;
}
