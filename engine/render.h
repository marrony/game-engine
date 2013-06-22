/*
 * render.h
 *
 *  Created on: 24/06/2013
 *      Author: marrony
 */

#ifndef RENDER_H_
#define RENDER_H_

#include "opengl.h"

#include <vector>

class EntitySystem;

class RenderSystem {
	ShaderSystem shader_system;
	std::vector<Material*> materials;
	std::vector<MeshLoaded*> meshes_loaded;
	std::vector<Model*> models;

	std::vector<Render> render_list;
	std::vector<Uniform> uniform_list;
	std::vector<Attribute> attribute_list;

	int width;
	int height;
public:
	void initialize(EntitySystem& entity_system);
	void finalize();

	void collect_attributes(Render& render, const MeshLoaded* mesh_loaded);

	void collect_uniforms(Render& render, int32_t node);

	void collect_render_commands();

	void render();
};

#endif /* RENDER_H_ */
