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

	fprintf(stderr, "wait for connection\n");
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
	fprintf(stderr, "initializing: %d %d %d\n", window_value->integer, width, height);
	engine.initialize((WindowID)(intptr_t)window_value->integer, width, height);
	fprintf(stderr, "initialized\n");

	json_free(json);
	fprintf(stderr, "initialized\n");

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
					} else if(!strcmp("create-model", json.data+type->string)) {
						Value* mesh = json_get_attribute(json, json.root, "mesh");
						Value* entity_name = json_get_attribute(json, json.root, "entity");
						Value* translation = json_get_attribute(json, json.root, "translation");
						Value* orientation = json_get_attribute(json, json.root, "orientation");
						Value* scale = json_get_attribute(json, json.root, "scale");

						Value* translation_x = json_get_at(json, *translation, 0);
						Value* translation_y = json_get_at(json, *translation, 1);
						Value* translation_z = json_get_at(json, *translation, 2);

						Value* scale_x = json_get_at(json, *scale, 0);
						Value* scale_y = json_get_at(json, *scale, 1);
						Value* scale_z = json_get_at(json, *scale, 2);

						Value* orientation_s = json_get_at(json, *orientation, 0);
						Value* orientation_x = json_get_at(json, *orientation, 1);
						Value* orientation_y = json_get_at(json, *orientation, 2);
						Value* orientation_z = json_get_at(json, *orientation, 3);

						Quaternion ori = Quaternion::make(orientation_s->number, orientation_x->number, orientation_y->number, orientation_z->number);
						Vector3 sc = Vector3::make(scale_x->number, scale_y->number, scale_z->number);
						Vector3 tr = Vector3::make(translation_x->number, translation_y->number, translation_z->number);

						int32_t entity = engine.create_entity(json.data+entity_name->string);
						engine.create_model(entity, json.data+mesh->string);
						engine.create_node(entity);

						Matrix4 m = Matrix4::transformationMatrix(ori, tr, sc);
						engine.transform_model(entity, m);
					}
				}

				json_free(json);
			}
		}

		fprintf(stderr, "run_one_frame()\n");
		engine.run_one_frame();
		fprintf(stderr, "run_one_framed()\n");
	}

	engine.finalize();

	protocol_send_finish_message(client);

	client.close();
	server.close();

	return 0;
}
