/*
 * render.h
 *
 *  Created on: 24/06/2013
 *      Author: marrony
 */

#ifndef RENDER_H_
#define RENDER_H_

#include "opengl.h"
#include "shader.h"
#include "mesh.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3.h"
#include "matrix4.h"

#include <vector>

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

class SceneGraph;
class EntitySystem;
class SwapChain;

class RenderSystem {
	EntitySystem* entity_system;
	ShaderSystem shader_system;
	std::vector<Material*> materials;
	std::vector<MeshLoaded*> meshes_loaded;
	std::vector<Model*> models;

	std::vector<Render> render_list;
	std::vector<Uniform> uniform_list;
	std::vector<Attribute> attribute_list;

	int width;
	int height;

	SwapChain* swap_chain;
	bool need_resize;

	int32_t load_mesh(const char* mesh_name);
public:
	int32_t MODEL_TYPE;

	void initialize(EntitySystem& entity_system, SwapChain* swap_chain);
	void finalize();

	int32_t create_model(int32_t entity, const char* mesh_name);

	void collect_attributes(Render& render, const MeshLoaded* mesh_loaded);

	void collect_uniforms(Render& render, SceneGraph& scene_graph, int32_t node);

	void collect_render_commands(SceneGraph& scene_graph);

	void resize(int width, int height) {
		this->width = width;
		this->height = height;
		need_resize = true;
	}

	void render();
};

#endif /* RENDER_H_ */
