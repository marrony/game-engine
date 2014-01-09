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

	engine.initialize((WindowID)(intptr_t)0, 600, 600);
	fprintf(stderr, "initialized\n");

//	engine.create_model(entity, json.data+mesh->string);
//	engine.create_node(entity);
//
	while(true) {
		engine.run_one_frame();
	}

	engine.finalize();

	return 0;
}
