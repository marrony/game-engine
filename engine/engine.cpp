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
#include "scene_graph.h"
#include "shader.h"

#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

class Engine {
	TaskManager task_manager;
	SceneGraph scene_graph;
	ShaderSystem shader_system;
	int32_t shader;
	ServerSocket server;
	Socket client;
	SwapChain swap_chain;
	Mesh* mesh;
	float ang;
	bool running;
	bool need_resize;

	int width;
	int height;

	void render() {
		//wglMakeCurrent(swap_chain.hdc, swap_chain.hglrc);

		if(need_resize) {
			swap_chain.resize(width, height);
			glViewport(0, 0, width, height);
			need_resize = false;
		}

		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(mesh) {
			shader_system.bind_shader(shader);
			Matrix4 projection = Matrix4::perspectiveMatrix(60, 1, 0.1, 1000);
			Matrix4 view = Matrix4::lookAtMatrix(Vector3(0, 0, 0), Vector3(0, 0, -1));
			Matrix4 model = Matrix4::transformationMatrix(Quaternion(Vector3(0, 1, 0), ang += 0.1), Vector3(0, -60, -200), Vector3(1, 1, 1));

			int projectionLocation = glGetUniformLocation(1, "projection");
			int modelLocation = glGetUniformLocation(1, "model");
			int viewLocation = glGetUniformLocation(1, "view");
			int attribLocation = glGetAttribLocation(1, "vPosition");

			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection.matrix);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.matrix);
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, view.matrix);

			glEnableVertexAttribArray(attribLocation);
			glVertexAttribPointer(attribLocation, 3, GL_FLOAT, GL_FALSE, 0, mesh->vertex_pointer());

			glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_SHORT, mesh->index_pointer());
		}

		swap_chain.swap_buffers();
	}

	static void render_function(TaskId task, WorkItem& item) {
		((Engine*)item.data)->render();
	}

	WorkItem render_task() {
		WorkItem item = {render_function, this};
		return item;
	}

	void update() {
		swap_chain.process_events();

		scene_graph.update();

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
						Value* width_value = json_get_attribute(json.value, "width");
						Value* height_value = json_get_attribute(json.value, "height");

						width = width_value->integer;
						height = height_value->integer;

						fprintf(stderr, "resize %dx%d\n", width, height);
						fflush(stderr);

						need_resize = true;
						//swap_chain_resize(swap_chain, width, height);
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
		need_resize = false;
		shader = -1;

		width = 0;
		height = 0;
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

		Value* window_value = json_get_attribute(json.value, "window");
		Value* width_value = json_get_attribute(json.value, "width");
		Value* height_value = json_get_attribute(json.value, "height");

		width = width_value->integer;
		height = height_value->integer;
		swap_chain.create((WindowID)(intptr_t)window_value->integer, width, height);

		json_free(json);

#define STRINGFY(x) #x

		Source sources[2];

		sources[0].type = VertexShader;
		sources[0].source = STRINGFY(
			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;
			attribute vec4 vPosition;

			void main() {
				gl_Position = projection * view * model * vPosition;
			}
		);

		sources[1].type = FragmentShader;
		sources[1].source = STRINGFY(
			void main() {
				gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
			}
		);

#undef STRINGFY

		shader = shader_system.create_shader("teste", 2, sources);
	}

	void run() {
		running = true;
		while(running) {
			TaskId update = task_manager.add(update_task(), INVALID_ID, pthread_self());
			TaskId render = task_manager.add(render_task(), update, pthread_self());

			task_manager.wait(update);
		}
	}

	void finalize() {
		swap_chain.destroy();

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
