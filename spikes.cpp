/*
 * spikes.cpp
 *
 *  Created on: 21/06/2013
 *      Author: marrony
 */

#include <stdint.h>
#include <vector>

#include "matrix4.h"

struct Engine {
	std::vector<const char*> component_types;
	std::vector<std::vector<int32_t> > ids;

	std::vector<const char*> entities;

	int32_t register_component(const char* component_type) {
		for(size_t i = 0; i < component_types.size(); i++) {
			if(!strcmp(component_types[i], component_type))
				return i;
		}

		int32_t index = component_types.size();
		component_types.push_back(component_type);
		ids.push_back(std::vector<int32_t>(entities.size(), -1));
		return index;
	}

	int32_t create_entity(const char* entity_name) {
		int32_t id = entities.size();

		entities.push_back(entity_name);
		for(int i = 0; i < ids.size(); i++)
			ids[i].push_back(-1);

		return id;
	}

	void add_component(int32_t entity, int32_t type, int32_t component) {
		ids[type][entity] = component;
	}
};

struct MeshSystem {
	int32_t load_mesh(const char* mesh) {
		return 0;
	}
};

struct PhysicSystem {
	int32_t create() {
		return 0;
	}

	void set_matrices(const std::vector<int32_t>& entities, const std::vector<Matrix4>& m) {
	}

	void get_matrices(const std::vector<int32_t>& entities, std::vector<Matrix4>& m) {
	}

	void update() {
	}
};

struct SceneGraph {
	std::vector<Matrix4> local;
	std::vector<Matrix4> world;
	std::vector<int32_t> parents;

	void update() {
	}

	int32_t create() {
		Matrix4 m = MATRIX4_IDENTITY;

		local.push_back(m);
		world.push_back(m);
		parents.push_back(0);

		return 0;
	}

	void get_world_matrices(const std::vector<int32_t>& entities, std::vector<Matrix4>& matrices) {
		for(size_t i = 0; i < entities.size(); i++) {
			int32_t id = entities[i];

			if(id == -1) continue;

			matrices[i] = world[id];
		}
	}

	void update_world_matrices(const std::vector<int32_t>& entities, const std::vector<Matrix4>& matrices) {
		for(size_t i = 0; i < entities.size(); i++) {
			int32_t id = entities[i];

			if(id == -1) continue;

			world[id] = matrices[i];
		}
	}
};

int main() {
	Engine engine;
	MeshSystem mesh_system;
	PhysicSystem physic_system;
	SceneGraph scene_graph;

	const int32_t Model = engine.register_component("Model");
	const int32_t Physic = engine.register_component("Physic");
	const int32_t Node = engine.register_component("Node");
	const int32_t Script = engine.register_component("Script");

	int32_t character = engine.create_entity("Character");

	int32_t model = mesh_system.load_mesh("my_mesh.mesh");
	int32_t physic = physic_system.create();
	int32_t node = scene_graph.create();

	engine.add_component(character, Model, model);
	engine.add_component(character, Physic, physic);
	engine.add_component(character, Node, node);

	const int32_t New = engine.register_component("New");

	engine.add_component(character, New, 10);

	int32_t character1 = engine.create_entity("Character1");

	std::vector<Matrix4> matrices(engine.entities.size());

	scene_graph.update();
	scene_graph.get_world_matrices(engine.ids[Node], matrices);
	physic_system.set_matrices(engine.ids[Physic], matrices);

	physic_system.update();
	physic_system.get_matrices(engine.ids[Physic], matrices);
	scene_graph.update_world_matrices(engine.ids[Node], matrices);

	return 0;
}
