/*
 * engine.h
 *
 *  Created on: 02/04/2013
 *      Author: marrony
 */

#ifndef ENGINE_H_
#define ENGINE_H_

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
#include <vector>
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

struct Pass {
	int32_t shader;
	int8_t texture_count;
	int16_t textures[16];

	struct {
		int8_t cull_face;
		int16_t face_mode;
		int16_t front_face;
	};

	struct {
		int8_t depth_test;
		int16_t depth_function;
	};

	struct {
		int8_t blend;
		int16_t blend_src;
		int16_t blend_dst;
		int16_t blend_equation;
	};

	struct {
		int8_t scissor;
		float scissor_offset_x, scissor_offset_y;
		float scissor_width, scissor_height;
	};

	struct {
		int8_t stencil;
		int16_t stencil_function;
		int32_t stencil_ref;
		uint32_t stencil_mask;
		int16_t stencil_fail;
		int16_t stencil_depth_fail;
		int16_t stencil_depth_pass;
	};
};

struct Material {
	int8_t pass_count;
	Pass passes[0];
};

struct Model {
	int32_t mesh;
	int32_t node;
	int8_t material_count;
	int16_t material[0];
};

struct Render {
	enum BoolType {
		Ignore,
		Enabled,
		Disabled
	};

	enum CommandType {
		ClearColor,
		ClearDepth,
		ClearColorAndDepth,
		Viewport,
	};

	uint64_t sort_key;

	union {
		struct {
			Batch batch;
			uint32_t index_buffer;
			uint32_t vertex_buffer;
			int32_t material_id;
			int32_t pass_id;
			int16_t attribute_start;
			int16_t attribute_end;
			int16_t uniform_start;
			int16_t uniform_end;
		};

		struct {
			CommandType command_type;
			union {
				struct {
					Vector4 clear_color;
					float clear_depth;
				};

				struct {
					float offset_x, offset_y;
					float scale_width, scale_height;
				};
			};
		};
	};
};

struct RenderListSort {
	bool operator()(const Render& r0, const Render& r1) const {
		return r0.sort_key < r1.sort_key;
	}
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
	SwapChain swap_chain;
	bool need_resize;

	std::vector<Material*> materials;
	std::vector<MeshLoaded*> meshes_loaded;
	std::vector<Model*> models;

	std::vector<Render> render_list;
	std::vector<Uniform> uniform_list;
	std::vector<Attribute> attribute_list;

	int width;
	int height;

	void collect_attributes(Render& render, const MeshLoaded* mesh_loaded);

	void collect_uniforms(Render& render, int32_t node);

	void collect_render_commands();

	void render();

	WorkItem render_task();

	void update();

	WorkItem update_task();

	static void render_function(TaskId task, WorkItem& item) {
		((Engine*)item.data)->render();
	}

	static void update_function(TaskId task, WorkItem& item) {
		((Engine*)item.data)->update();
	}

public:
	Engine();

	void load_mesh(const char* mesh_name);
	void resize(int width, int height);

	void initialize(WindowID handle, int width, int height);
	void finalize();

	void run_one_frame();
};

#endif /* ENGINE_H_ */
