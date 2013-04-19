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
#include "task.h"

#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

class Engine {
	TaskManager task_manager;
	ServerSocket server;
	Socket client;
	SwapChain swap_chain;
	Mesh* mesh;
	float ang;
	bool running;

	void render() {
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

	static void render_function(TaskId task, WorkItem& item) {
		((Engine*)item.data)->render();
	}

	WorkItem render_task() {
		WorkItem item = {render_function, this};
		return item;
	}

	void update() {
		swap_chain_process_events(swap_chain);

		if(client.has_data()) {
			Json json;

			if(!protocol_recv_message(client, json))
				running = false;
			else {
				Value* type = json_get_attribute(json.value, "type");

				if(type) {
					if(!strcmp("finish", type->string)) {
						running = false;

						fprintf(stderr, "type: %s\n", type->string);
						fflush(stderr);
					} else if(!strcmp("resize", type->string)) {
						Value* width = json_get_attribute(json.value, "width");
						Value* height = json_get_attribute(json.value, "height");

						fprintf(stderr, "resize %dx%d\n", width->integer, height->integer);
						fflush(stderr);

						swap_chain_resize(swap_chain, width->integer, height->integer);
						glViewport(0, 0, width->integer, height->integer);
					} else if(!strcmp("load-mesh", type->string)) {
						Value* _mesh = json_get_attribute(json.value, "mesh");

						if(mesh)
							mesh_destroy(mesh);

						mesh = mesh_read(_mesh->string);
					}
				}

				json_free(json);
			}
		}
	}

	static void update_function(TaskId task, WorkItem& item) {
		((Engine*)item.data)->update();
	}

	WorkItem update_task() {
		WorkItem item = {update_function, this};
		return item;
	}
public:
	Engine() : task_manager(32) {
		mesh = 0;
		ang = 0;
		running = false;
	}

	void initialize(short port) {
		server.create(port);

		fprintf(stderr, "waint for connection\n");
		fflush(stderr);

		client = server.accept();

		fprintf(stderr, "connected: %d\n", *(int*)&client);
		fflush(stderr);

		Json json;
		protocol_recv_message(client, json);

		Value* window = json_get_attribute(json.value, "window");
		Value* width = json_get_attribute(json.value, "width");
		Value* height = json_get_attribute(json.value, "height");

		swap_chain = swap_chain_create((WindowID)(intptr_t)window->integer, width->integer, height->integer);

		json_free(json);
	}

	void run() {
		running = true;
		while(running) {
			TaskId update = task_manager.begin_add(update_task());
			{
				TaskId render = task_manager.begin_add(render_task(), update);
				task_manager.finish_add(render);
			}
			task_manager.finish_add(update);
			task_manager.wait(update);
		}
	}

	void finalize() {
		swap_chain_destroy(swap_chain);

		fprintf(stderr, "finish engine\n");
		fflush(stderr);

		protocol_send_finish_message(client);

		if(mesh)
			mesh_destroy(mesh);

		client.close();
		server.close();
	}
};

int get_argument(int argc, char* argv[], const char* arg_name) {
	for(int i = 0; i < argc; i++) {
		if(!strcmp(arg_name, argv[i]) && i+1 < argc)
			return atoi(argv[i+1]);
	}

	return -1;
}

int main(int argc, char* argv[]) {
	int port = get_argument(argc, argv, "--port");

	Engine engine;

	engine.initialize(port < 0 ? 9090 : port);
	engine.run();
	engine.finalize();

	return 0;
}
