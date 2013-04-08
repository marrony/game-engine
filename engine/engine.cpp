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

#include <cstring>
#include <stdlib.h>
#include <stdio.h>

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

	char buffer[1024];
	int nbytes = client.recv(buffer, sizeof(buffer));
	buffer[nbytes]= 0;

	fprintf(stderr, "%s\n", buffer);
	fflush(stderr);

	Json json = json_parse(buffer, nbytes);
	Value* window = json_get_attribute(json.value, "window");
	Value* width = json_get_attribute(json.value, "width");
	Value* height = json_get_attribute(json.value, "height");

	SwapChain swap_chain = swap_chain_create((WindowID)window->integer, width->integer, height->integer);

	json_free(json);

	bool running = true;
	while(running) {
		swap_chain_process_events(swap_chain);

		if(client.has_data()) {
			int nbytes = client.recv(buffer, sizeof(buffer));

			fprintf(stderr, "bytes read: %d\n", nbytes);
			fflush(stderr);

			if(nbytes < 0)
				running = false;
			else if(nbytes > 0) {
				buffer[nbytes]= 0;

				fprintf(stderr, "data read: %s\n", buffer);
				fflush(stderr);

				json = json_parse(buffer, nbytes);

				Value* type = json_get_attribute(json.value, "type");
				if(type && !strcmp("finish", type->string)) {
					running = false;

					fprintf(stderr, "type: %s\n", type->string);
					fflush(stderr);
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

		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		glVertex3f(-.75, -.75, -1.0);
		glColor3f(0, 1, 0);
		glVertex3f(.75, -.75, -1.0);
		glColor3f(0, 0, 1);
		glVertex3f(.75, .75, -1.0);
		glEnd();

		swap_chain_swap_buffers(swap_chain);
	}

	swap_chain_destroy(swap_chain);

	fprintf(stderr, "finish engine\n");
	fflush(stderr);

	snprintf(buffer, sizeof(buffer), "{\"type\": \"finish\"}");
	client.send(buffer, strlen(buffer));

	client.close();
	server.close();

	return 0;
}
