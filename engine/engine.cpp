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
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);

			struct Attrib {
				int32_t index;
				int32_t size;
				intptr_t pointer;
			};

			struct Uniform {
				int8_t type;
				int16_t count;
				union {
					Vector3 vec3;
					Vector4 vec4;
					Matrix3 mat3;
					Matrix4 mat4;
				};
			};

			struct Material {
				int32_t shader;
				int16_t attrib_count;
				Attrib* attribs;
			};

			struct Model {
				Mesh* mesh;
				int16_t material_count;
				Material* materials;
			};

			shader_system.bind_shader(shader);
			Matrix4 projection = Matrix4::perspectiveMatrix(60, 1, 0.1, 1000);
			Matrix4 view = Matrix4::lookAtMatrix(Vector3(0, 0, 0), Vector3(0, 0, -1));
			Matrix4 model = Matrix4::transformationMatrix(Quaternion(Vector3(0, 1, 0), ang += 0.1), Vector3(0, -60, -200), Vector3(1, 1, 1));
			Matrix4 modelView = view * model;
			Matrix3 normalMatrix = modelView.upperLeft().inverse().transpose();

			int normalMatrixLocation = glGetUniformLocation(1, "normalMatrix");
			int projectionMatrixLocation = glGetUniformLocation(1, "projectionMatrix");
			int modelViewMatrixLocation = glGetUniformLocation(1, "modelViewMatrix");
			int lightLocation = glGetUniformLocation(1, "light");

			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, projection.matrix);
			glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, modelView.matrix);
			glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, normalMatrix.matrix);
			glUniform3f(lightLocation, 0, 0, 0);

			int positionLocation = glGetAttribLocation(1, "vPosition");
			int normalLocation = glGetAttribLocation(1, "vNormal");

			glEnableVertexAttribArray(positionLocation);
			glEnableVertexAttribArray(normalLocation);

			glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, mesh->vertex_pointer());
			glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, mesh->normal_pointer());

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

		sources[1].type = FragmentShader;
		sources[1].source = STRINGFY(
			uniform vec3 light;

			varying vec3 N;
			varying vec4 V;

			void main() {
				vec3 light_dir = normalize(light - V.xyz);
				vec3 normal = normalize(N);

				gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0) * clamp(dot(normal, light_dir), 0, 1);
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
