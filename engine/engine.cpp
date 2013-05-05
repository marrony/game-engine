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

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3.h"
#include "matrix4.h"

#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

struct Attribute {
	int32_t index;
	int32_t size;
	int32_t stride;
	int32_t pointer;
};

struct Uniform {
	int32_t index;
	int32_t type;
	int16_t count;
	union {
		float* pointer;
		float vec1;
		Vector2 vec2;
		Vector3 vec3;
		Vector4 vec4;
		Matrix3 mat3;
		Matrix4 mat4;
	};
};

struct Material {
	int32_t shader;
	int8_t texture_count;
	int32_t textures[16];
};

struct Model {
	int32_t mesh;
};

struct Render {
	uint16_t offset;
	uint16_t count;
	uint16_t start;
	uint16_t end;
	uint32_t index_buffer;
	uint32_t vertex_buffer;
	int32_t shader;
	int16_t attribute_start;
	int16_t attribute_end;
	int16_t uniform_start;
	int16_t uniform_end;
};

struct MeshLoaded {
	char name[32];
	uint32_t vertex_buffer;
	uint32_t index_buffer;
	int32_t offsets[Mesh::MaxAttributes];
	int8_t batch_count;
	Batch batches[0];
};

class Engine {
	TaskManager task_manager;
	SceneGraph scene_graph;
	ShaderSystem shader_system;
	int32_t shader;
	ServerSocket server;
	Socket client;
	SwapChain swap_chain;
	float ang;
	bool running;
	bool need_resize;

	std::vector<Material> materials;
	std::vector<MeshLoaded*> meshes_loaded;
	std::vector<Model*> models;

	std::vector<Render> render_list;
	std::vector<Uniform> uniform_list;
	std::vector<Attribute> attribute_list;

	int width;
	int height;

	void load_mesh(const char* mesh_name) {
		for(size_t i = 0; i < meshes_loaded.size(); i++) {
			if(!strcmp(meshes_loaded[i]->name, mesh_name))
				return;
		}

		Mesh* mesh = mesh_read(mesh_name);

		MeshLoaded* mesh_loaded = (MeshLoaded*)malloc(sizeof(MeshLoaded) + sizeof(Batch)*mesh->batch_count);
		strcpy(mesh_loaded->name, mesh_name);
		mesh_loaded->batch_count = mesh->batch_count;
		memcpy(mesh_loaded->batches, mesh->batches_pointer(), sizeof(Batch)*mesh->batch_count);

		for(int i = 0; i < Mesh::MaxAttributes; i++) {
			if(mesh->offsets[i] != -1)
				mesh_loaded->offsets[i] = mesh->offsets[i] - mesh->index_size();
			else
				mesh_loaded->offsets[i] = -1;
		}

		glGenBuffers(1, &mesh_loaded->vertex_buffer);
		glGenBuffers(1, &mesh_loaded->index_buffer);

		glBindBuffer(GL_ARRAY_BUFFER, mesh_loaded->vertex_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_loaded->index_buffer);

		glBufferData(GL_ARRAY_BUFFER, mesh->vertex_size(), mesh->vertex_pointer(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->index_size(), mesh->index_pointer(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		Model* model = (Model*)malloc(sizeof(Model));
		model->mesh = meshes_loaded.size();

		size_t offset = materials.size();
		for(int8_t i = 0; i < mesh_loaded->batch_count; i++) {
			mesh_loaded->batches[i].material += offset;
		}

		int16_t material_count = mesh->material_count();
		for(int16_t i = 0; i < material_count; i++) {
			Material material = {shader};
			materials.push_back(material);
		}

		meshes_loaded.push_back(mesh_loaded);
		models.push_back(model);

		mesh_destroy(mesh);
	}

	void collect_attributes(Render& render, const MeshLoaded* mesh_loaded) {
		std::vector<Location> locations = shader_system.get_attributes(render.shader);

		render.attribute_start = attribute_list.size();
		render.attribute_end = render.attribute_start + locations.size();

		attribute_list.resize(render.attribute_end);
		for(int i = 0; i < locations.size(); i++) {
			const Location& location = locations[i];
			Mesh::Attributes semantic = (Mesh::Attributes)location.semantic;

			attribute_list[render.attribute_start+i].index = location.index;
			attribute_list[render.attribute_start+i].pointer = mesh_loaded->offsets[semantic];
			attribute_list[render.attribute_start+i].size = Mesh::get_size(semantic);
			attribute_list[render.attribute_start+i].stride = 0;
		}
	}

	void collect_uniforms(Render& render) {
		Vector3 cam[] = {
				{0, -5, -10},
				{0, -5, -200},
		};

		Matrix4 projectionMatrix = Matrix4::perspectiveMatrix(60, (float)width/(float)height, 0.1, 1000);
		Matrix4 viewMatrix = Matrix4::lookAtMatrix(vector::make(0, 0, 0), vector::make(0, 0, -1));
		Matrix4 modelMatrix = Matrix4::transformationMatrix(Quaternion(vector::make(0, 1, 0), ang), cam[models.size() != 1], vector::make(1, 1, 1));
		Matrix4 modelViewMatrix = viewMatrix * modelMatrix;
		Matrix3 normalMatrix = modelViewMatrix.upperLeft().inverse().transpose();

		std::vector<Location> uniforms = shader_system.get_uniforms(render.shader);

		render.uniform_start = uniform_list.size();
		render.uniform_end = render.uniform_start + uniforms.size();

		uniform_list.resize(render.uniform_end);
		for(int j = 0; j < uniforms.size(); j++) {
			uniform_list[render.uniform_start+j].index = uniforms[j].index;
			uniform_list[render.uniform_start+j].type = uniforms[j].type;

			switch(uniforms[j].semantic) {
			case ProjectionMatrix:
				uniform_list[render.uniform_start+j].mat4 = projectionMatrix;
				break;

			case ViewMatrix:
				uniform_list[render.uniform_start+j].mat4 = viewMatrix;
				break;

			case ModelMatrix:
				uniform_list[j].mat4 = modelMatrix;
				break;

			case ModelViewMatrix:
				uniform_list[render.uniform_start+j].mat4 = modelViewMatrix;
				break;

			case NormalMatrix:
				uniform_list[render.uniform_start+j].mat3 = normalMatrix;
				break;

			case LightPosition:
				uniform_list[render.uniform_start+j].vec3 = vector::make(0, 0, 0);
				break;
			}
		}
	}

	void render() {
		if(need_resize) {
			swap_chain.resize(width, height);
			glViewport(0, 0, width, height);
			need_resize = false;
		}

		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(!models.empty()) {
			ang += 0.1;

			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);

			render_list.clear();
			uniform_list.clear();
			attribute_list.clear();

			for(size_t j = 0; j < models.size(); j++) {
				const Model* model = models[j];
				const MeshLoaded* mesh_loaded = meshes_loaded[model->mesh];

				for(int i = 0; i < mesh_loaded->batch_count; i++) {
					render_list.push_back(Render());

					Render& render = render_list.back();

					render.index_buffer = mesh_loaded->index_buffer;
					render.vertex_buffer = mesh_loaded->vertex_buffer;
					render.offset = mesh_loaded->batches[i].offset;
					render.count = mesh_loaded->batches[i].count;
					render.start = mesh_loaded->batches[i].start;
					render.end = mesh_loaded->batches[i].end;
					render.shader = materials[mesh_loaded->batches[i].material].shader;
					collect_attributes(render, mesh_loaded);
					collect_uniforms(render);
				}
			}

			//render

			for(int i = 0; i < render_list.size(); i++) {
				Render& r = render_list[i];

				shader_system.bind_shader(r.shader);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.index_buffer);
				glBindBuffer(GL_ARRAY_BUFFER, r.vertex_buffer);

				for(int j = r.uniform_start; j < r.uniform_end; j++) {
					Uniform& u = uniform_list[j];

					switch(u.type) {
					case GL_FLOAT:
						break;
					case GL_FLOAT_VEC2:
						break;
					case GL_FLOAT_VEC3:
						glUniform3fv(u.index, 1, u.vec3.vector);
						break;
					case GL_FLOAT_VEC4:
						break;
					case GL_FLOAT_MAT2:
						break;
					case GL_FLOAT_MAT3:
						glUniformMatrix3fv(u.index, 1, GL_FALSE, u.mat3.matrix);
						break;
					case GL_FLOAT_MAT4:
						glUniformMatrix4fv(u.index, 1, GL_FALSE, u.mat4.matrix);
						break;
					}
				}

				for(int j = r.attribute_start; j < r.attribute_end; j++) {
					const Attribute& attribute = attribute_list[j];

					glEnableVertexAttribArray(attribute.index);
					glVertexAttribPointer(attribute.index, attribute.size, GL_FLOAT, GL_FALSE, attribute.stride, (char*)0 + attribute.pointer);
				}

				glDrawRangeElements(GL_TRIANGLES, r.start, r.end, r.count, GL_UNSIGNED_SHORT, (uint16_t*)0 + r.offset);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
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
						Value* mesh = json_get_attribute(json.value, "mesh");
						load_mesh(mesh->string);
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
			uniform vec3 lightPosition;

			varying vec3 N;
			varying vec4 V;

			void main() {
				vec3 light_dir = normalize(lightPosition - V.xyz);
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
