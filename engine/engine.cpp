/*
 * engine.cpp
 *
 *  Created on: 01/04/2013
 *      Author: marrony
 */

#include "engine.h"
#include "swap_chain.h"
#include "socket.h"
#include "json.h"
#include "protocol.h"
#include "mesh.h"
#include "mesh_io.h"

#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

int get_argument(int argc, char* argv[], const char* arg_name) {
	for(int i = 0; i < argc; i++) {
		if(!strcmp(arg_name, argv[i]) && i+1 < argc)
			return atoi(argv[i+1]);
	}

	return -1;
}

int main(int argc, char* argv[]) {
	int port = get_argument(argc, argv, "--port");

	ServerSocket server(port < 0 ? 9090 : port);

	fprintf(stderr, "waint for connection\n");
	fflush(stderr);

	Socket client = server.accept();

	fprintf(stderr, "connected: %d\n", *(int*)&client);
	fflush(stderr);

	char buffer[MAX_PROTOCOL_PACKET_SIZE] = {0};
	Mesh* mesh = 0;
	size_t nbytes;

	protocol_recv_packet(client, buffer, sizeof(buffer), nbytes);
	buffer[nbytes] = 0;

	fprintf(stderr, "read: %s\n", buffer);
	fflush(stderr);

	Json json = json_parse(buffer, nbytes);
	Value* window = json_get_attribute(json.value, "window");
	Value* width = json_get_attribute(json.value, "width");
	Value* height = json_get_attribute(json.value, "height");

	SwapChain swap_chain = swap_chain_create((WindowID)window->integer, width->integer, height->integer);

	json_free(json);

	float ang = 0;

	bool running = true;
	while(running) {
		swap_chain_process_events(swap_chain);

		if(client.has_data()) {
			if(protocol_recv_packet(client, buffer, sizeof(buffer), nbytes) <= 0)
				running = false;
			else if(nbytes > 0) {
				buffer[nbytes] = 0;

				fprintf(stderr, "data read: %s\n", buffer);
				fflush(stderr);

				json = json_parse(buffer, nbytes);

				Value* type = json_get_attribute(json.value, "type");
				if(type) {
					if(!strcmp("finish", type->string)) {
						running = false;

						fprintf(stderr, "type: %s\n", type->string);
						fflush(stderr);
					} else if(!strcmp("resize", type->string)) {
						Value* width = json_get_attribute(json.value, "width");
						Value* height = json_get_attribute(json.value, "height");

						swap_chain_resize(swap_chain, width->integer, height->integer);
						glViewport(0, 0, width->integer, height->integer);
					} else if(!strcmp("load-mesh", type->string)) {
						Value* _mesh = json_get_attribute(json.value, "mesh");

						mesh = mesh_read(_mesh->string);
					}
				}

				json_free(json);
			}
		}

		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1., 1., -1., 1., 1., 20.);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);
		glRotatef(ang += 0.02, 0, 0, 1);

		if(mesh) {
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, mesh->vertex_pointer());
			glColorPointer(3, GL_FLOAT, 0, mesh->color_pointer());
			glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_SHORT, mesh->index_pointer());
		}

		swap_chain_swap_buffers(swap_chain);
	}

	swap_chain_destroy(swap_chain);

	fprintf(stderr, "finish engine\n");
	fflush(stderr);

	snprintf(buffer, sizeof(buffer), "{\"type\": \"finish\"}");
	protocol_send_packet(client, buffer, strlen(buffer));

	free(mesh);
	client.close();
	server.close();

	return 0;
}
