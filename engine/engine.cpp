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

#include <algorithm>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

enum CommandType {
	PreDraw  = 0,
	Draw     = 1,
	PostDraw = 2,
};

const uint64_t PASS_MASK     = 0x000000000000000fLL;
const uint64_t MATERIAL_MASK = 0x00000000000ffff0LL;
const uint64_t DEPTH_MASK    = 0x000000fffff00000LL;
const uint64_t COMMAND_MASK  = 0x0000030000000000LL;
const uint64_t SEQUENCE_MASK = 0x000000ffffffffffLL;

inline uint64_t PASS_BITS(uint64_t pass) {
	return pass & PASS_MASK;
}

inline uint64_t MATERIAL_BITS(uint64_t material) {
	return (material << 4) & MATERIAL_MASK;
}

inline uint64_t DEPTH_BITS(uint64_t depth) {
	return (depth << 20) & DEPTH_MASK;
}

inline bool IS_COMMAND(uint64_t key, uint64_t command_type) {
	return (key & COMMAND_MASK) == (command_type << 40);
}

inline uint64_t COMMAND_BITS(uint64_t cmd) {
	return (cmd << 40) & COMMAND_MASK;
}

inline uint64_t SEQUENCE_BITS(uint64_t sequence) {
	return sequence & SEQUENCE_MASK;
}

void Engine::load_mesh(const char* mesh_name) {
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

	Model* model = (Model*)malloc(sizeof(Model) + sizeof(int16_t)*mesh_loaded->batch_count);
	model->material_count = mesh_loaded->batch_count;
	model->mesh = meshes_loaded.size();

	for(int8_t i = 0; i < mesh_loaded->batch_count; i++) {
		model->material[i] = i % materials.size();
	}

	meshes_loaded.push_back(mesh_loaded);
	models.push_back(model);

	mesh_destroy(mesh);
}

void Engine::resize(int width, int height) {
	this->width = width;
	this->height = height;
	need_resize = true;
}

void Engine::collect_attributes(Render& render, const MeshLoaded* mesh_loaded) {
	const Material* material = materials[render.material_id];
	const std::vector<Location> locations = shader_system.get_attributes(material->passes[render.pass_id].shader);

	render.attribute_start = attribute_list.size();
	render.attribute_end = render.attribute_start + locations.size();

	attribute_list.resize(render.attribute_end);
	for(int i = 0; i < locations.size(); i++) {
		const Location& location = locations[i];
		Mesh::Attributes semantic = (Mesh::Attributes)location.semantic;
		Attribute& attribute = attribute_list[render.attribute_start+i];

		attribute.index = location.index;
		attribute.pointer = mesh_loaded->offsets[semantic];
		attribute.size = Mesh::get_size(semantic);
		attribute.stride = 0;
	}
}

void Engine::collect_uniforms(Render& render) {
	Vector3 cam[] = {
			{0, -5, -10},
			{0, -5, -200},
	};

	Matrix4 projectionMatrix = Matrix4::perspectiveMatrix(60, (float)width/(float)height, 0.1, 1000);
	Matrix4 viewMatrix = Matrix4::lookAtMatrix(Vector3::make(0, 0, 0), Vector3::make(0, 0, -1));
	Matrix4 modelMatrix = Matrix4::transformationMatrix(Quaternion(Vector3::make(0, 1, 0), ang), cam[models.size() != 1], Vector3::make(1, 1, 1));
	Matrix4 modelViewMatrix = viewMatrix * modelMatrix;
	Matrix3 normalMatrix = modelViewMatrix.upperLeft().inverse().transpose();

	const Material* material = materials[render.material_id];
	const std::vector<Location> uniforms = shader_system.get_uniforms(material->passes[render.pass_id].shader);

	render.uniform_start = uniform_list.size();
	render.uniform_end = render.uniform_start + uniforms.size();

	uniform_list.resize(render.uniform_end);
	for(int j = 0; j < uniforms.size(); j++) {
		Uniform& uniform = uniform_list[render.uniform_start+j];

		uniform.index = uniforms[j].index;
		uniform.type = uniforms[j].type;

		switch(uniforms[j].semantic) {
		case ProjectionMatrix:
			uniform.mat4 = projectionMatrix;
			break;

		case ViewMatrix:
			uniform.mat4 = viewMatrix;
			break;

		case ModelMatrix:
			uniform_list[j].mat4 = modelMatrix;
			break;

		case ModelViewMatrix:
			uniform.mat4 = modelViewMatrix;
			break;

		case NormalMatrix:
			uniform.mat3 = normalMatrix;
			break;

		case LightPosition:
			uniform.vec3 = Vector3::make(0, 0, 0);
			break;
		}
	}
}

void Engine::collect_render_commands() {
	render_list.clear();
	uniform_list.clear();
	attribute_list.clear();

	int sequence = 0;

	if(need_resize) {
		swap_chain.resize(width, height);
		need_resize = false;

		render_list.push_back(Render());
		render_list.back().sort_key = COMMAND_BITS(PreDraw) | SEQUENCE_BITS(sequence++);
		render_list.back().command_type = Render::Viewport;
		render_list.back().offset_x = 0;
		render_list.back().offset_y = 0;
		render_list.back().scale_width = 1;
		render_list.back().scale_height = 1;
	}

	render_list.push_back(Render());
	render_list.back().sort_key = COMMAND_BITS(PreDraw) | SEQUENCE_BITS(sequence++);
	render_list.back().command_type = Render::ClearColorAndDepth;
	render_list.back().clear_color = Vector4::make(1.0, 1.0, 1.0, 1.0);
	render_list.back().clear_depth = 1.0;

	if(!models.empty()) {
		ang += 0.1;

		for(size_t j = 0; j < models.size(); j++) {
			const Model* model = models[j];
			const MeshLoaded* mesh_loaded = meshes_loaded[model->mesh];

			for(int i = 0; i < mesh_loaded->batch_count; i++) {
				const int16_t material_id = model->material[i];
				const Material* material = materials[material_id];

				for(int pass_id = 0; pass_id < material->pass_count; pass_id++) {
					render_list.push_back(Render());

					Render& render = render_list.back();

					render.sort_key = COMMAND_BITS(Draw) | MATERIAL_BITS(material_id) | PASS_BITS(pass_id);
					render.index_buffer = mesh_loaded->index_buffer;
					render.vertex_buffer = mesh_loaded->vertex_buffer;
					render.batch = mesh_loaded->batches[i];
					render.material_id = material_id;
					render.pass_id = pass_id;
					collect_attributes(render, mesh_loaded);
					collect_uniforms(render);
				}
			}
		}
	}

	std::sort(render_list.begin(), render_list.end(), RenderListSort());
}

void Engine::render() {
	glScissor(0, 0, width, height);
	glViewport(0, 0, width, height);

	for(size_t i = 0; i < render_list.size(); i++) {
		Render& r = render_list[i];

		if(!IS_COMMAND(r.sort_key, Draw)) {
			switch(r.command_type) {
			case Render::ClearColor:
				glClearColor(r.clear_color.r, r.clear_color.g, r.clear_color.b, r.clear_color.a);
				glClear(GL_COLOR_BUFFER_BIT);
				break;
			case Render::ClearDepth:
				glClearDepthf(r.clear_depth);
				glClear(GL_DEPTH_BUFFER_BIT);
				break;
			case Render::ClearColorAndDepth:
				glClearColor(r.clear_color.r, r.clear_color.g, r.clear_color.b, r.clear_color.a);
				glClearDepthf(r.clear_depth);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				break;
			case Render::Viewport:
				glViewport(width*r.offset_x, height*r.offset_y, width*r.scale_width, height*r.scale_height);
				break;
			}
		} else {
			const Material* material = materials[r.material_id];
			const Pass* pass = material->passes+r.pass_id;
			shader_system.bind_shader(pass->shader);

			if(pass->cull_face == Render::Enabled) {
				glEnable(GL_CULL_FACE);

				glCullFace(pass->face_mode);
				glFrontFace(pass->front_face);
			} else if(pass->cull_face == Render::Disabled)
				glDisable(GL_CULL_FACE);

			if(pass->depth_test == Render::Enabled) {
				glEnable(GL_DEPTH_TEST);

				glDepthFunc(pass->depth_function);
			} else if(pass->depth_test == Render::Disabled)
				glDisable(GL_DEPTH_TEST);

			if(pass->blend == Render::Enabled) {
				glEnable(GL_BLEND);

				glBlendFunc(pass->blend_src, pass->blend_dst);
				glBlendEquation(pass->blend_equation);
			} else if(pass->blend == Render::Disabled)
				glDisable(GL_BLEND);

			if(pass->scissor == Render::Enabled) {
				glEnable(GL_SCISSOR_TEST);

				glScissor(width*pass->scissor_offset_x, height*pass->scissor_offset_y, width*pass->scissor_width, height*pass->scissor_height);
			} else if(pass->scissor == Render::Disabled)
				glDisable(GL_SCISSOR_TEST);

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

#ifdef ANDROID
			glDrawElements(GL_TRIANGLES, r.batch.count, GL_UNSIGNED_SHORT, (uint16_t*)0 + r.batch.offset);
#else
			glDrawRangeElements(GL_TRIANGLES, r.batch.start, r.batch.end, r.batch.count, GL_UNSIGNED_SHORT, (uint16_t*)0 + r.batch.offset);
#endif
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	swap_chain.swap_buffers();
}

WorkItem Engine::render_task() {
	WorkItem item = {render_function, this};
	return item;
}

void Engine::update() {
	swap_chain.process_events();

	scene_graph.update();

	collect_render_commands();
}

WorkItem Engine::update_task() {
	WorkItem item = {update_function, this};
	return item;
}

Engine::Engine() : task_manager(32) {
	ang = 0;
	need_resize = true;

	width = 0;
	height = 0;
}

void Engine::initialize(WindowID handle, int width, int height) {
	this->width = width;
	this->height = height;

	swap_chain.create(handle, width, height);

#define STRINGFY(x) #x

	Source sources0[2];
	Source sources1[2];

	sources0[0].type = VertexShader;
	sources0[0].source = STRINGFY(
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

	sources0[1].type = FragmentShader;
	sources0[1].source = STRINGFY(
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

	sources1[0].type = VertexShader;
	sources1[0].source = STRINGFY(
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

	sources1[1].type = FragmentShader;
	sources1[1].source = STRINGFY(
		uniform vec3 lightPosition;

		varying vec3 N;
		varying vec4 V;

		void main() {
			vec3 light_dir = normalize(lightPosition - V.xyz);
			vec3 normal = normalize(N);

			float shade = clamp(dot(normal, light_dir), 0, 1);
			gl_FragColor = vec4(0.0, 0.0, 1.0, 0.5) * vec4(vec3(shade), 1);
		}
	);

#undef STRINGFY

	int32_t shader0 = shader_system.create_shader("shader0", 2, sources0);
	int32_t shader1 = shader_system.create_shader("shader1", 2, sources1);

	Material* material0 = (Material*)malloc(sizeof(Material) + sizeof(Pass)*2);
	material0->pass_count = 1;

	material0->passes[0].shader = shader0;
	material0->passes[0].cull_face = Render::Enabled;
	material0->passes[0].face_mode = GL_BACK;
	material0->passes[0].front_face = GL_CCW;
	material0->passes[0].depth_test = Render::Enabled;
	material0->passes[0].depth_function = GL_LEQUAL;
	material0->passes[0].blend = Render::Enabled;
	material0->passes[0].blend_src = GL_SRC_ALPHA;
	material0->passes[0].blend_dst = GL_ONE_MINUS_SRC_ALPHA;
	material0->passes[0].blend_equation = GL_FUNC_ADD;
	material0->passes[0].scissor = Render::Ignore;
	material0->passes[0].scissor_offset_x = 0.25f;
	material0->passes[0].scissor_offset_y = 0.25f;
	material0->passes[0].scissor_height = 0.5f;
	material0->passes[0].scissor_width = 0.5f;

	material0->passes[1].shader = shader0;
	material0->passes[1].cull_face = Render::Enabled;
	material0->passes[1].face_mode = GL_BACK;
	material0->passes[1].front_face = GL_CCW;
	material0->passes[1].depth_test = Render::Enabled;
	material0->passes[1].depth_function = GL_LEQUAL;
	material0->passes[1].blend = Render::Disabled;
	material0->passes[1].blend_src = GL_SRC_ALPHA;
	material0->passes[1].blend_dst = GL_ONE;
	material0->passes[1].blend_equation = GL_FUNC_ADD;

	materials.push_back(material0);

	Material* material1 = (Material*)malloc(sizeof(Material) + sizeof(Pass)*2);
	material1->pass_count = 1;

	material1->passes[0].shader = shader1;
	material1->passes[0].cull_face = Render::Disabled;
	material1->passes[0].face_mode = GL_BACK;
	material1->passes[0].front_face = GL_CCW;
	material1->passes[0].depth_test = Render::Enabled;
	material1->passes[0].depth_function = GL_LEQUAL;
	material1->passes[0].blend = Render::Disabled;
	material1->passes[0].blend_src = GL_SRC_ALPHA;
	material1->passes[0].blend_dst = GL_ONE_MINUS_SRC_ALPHA;
	material1->passes[0].blend_equation = GL_FUNC_ADD;
	material1->passes[0].scissor = Render::Ignore;
	material1->passes[0].scissor_offset_x = 0.25f;
	material1->passes[0].scissor_offset_y = 0.25f;
	material1->passes[0].scissor_height = 0.5f;
	material1->passes[0].scissor_width = 0.5f;
	materials.push_back(material1);
}

void Engine::runOneFrame() {
	TaskId update = task_manager.add(update_task(), INVALID_ID, pthread_self());
	TaskId render = task_manager.add(render_task(), update, pthread_self());

	task_manager.wait(update);
}

void Engine::finalize() {
	swap_chain.destroy();

	fprintf(stderr, "finish engine\n");
	fflush(stderr);
}
