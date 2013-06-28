/*
 * scene_graph.cpp
 *
 *  Created on: 24/05/2013
 *      Author: marrony
 */

#include "scene_graph.h"
#include "entity.h"

void SceneGraph::initialize(EntitySystem& entity_system) {
	this->entity_system = &entity_system;

	Matrix4 m = MATRIX4_IDENTITY;

	local.push_back(m);
	world.push_back(m);
	parents.push_back(0);
	entities.push_back(-1);

	local.push_back(m);
	world.push_back(m);
	parents.push_back(0);
	entities.push_back(-1);

	TYPE = entity_system.register_component("SceneNode");
}

void SceneGraph::finalize() {
}

void SceneGraph::update() {
	for(size_t i = 1; i < local.size(); i++) {
		int p = parents[i];
		world[i] = world[p] * local[i];
	}
}

Matrix4 SceneGraph::get_local_matrix(int32_t node) const {
	return local[node];
}

Matrix4 SceneGraph::get_world_matrix(int32_t node) const {
	return world[node];
}

int32_t SceneGraph::create_node(int32_t entity, int32_t parent) {
	int32_t index = 2;

	for(; index < parents.size(); index++) {
		if(parents[index] == 0)
			break;
	}

	Matrix4 m = MATRIX4_IDENTITY;

	if(index <= parent || index >= parents.size()) {
		index = parents.size();

		local.push_back(m);
		world.push_back(m);
		parents.push_back(parent);
		entities.push_back(entity);
	} else {
		local[index] = m;
		world[index] = m;
		parents[index] = parent;
		entities[index] = entity;
	}

	entity_system->add_component(entity, TYPE, index);

	return index;
}

void SceneGraph::destroy_node(int32_t node) {
	parents[node] = 0;
}

void SceneGraph::transform_node(int32_t node, const Matrix4& m) {
	local[node] = local[node] * m;
}

std::vector<int32_t> SceneGraph::get_visible_entities() {
	std::vector<int32_t> visible_entities;

	for(size_t i = 0; i < entities.size(); i++) {
		if(entities[i] != -1)
			visible_entities.push_back(entities[i]);
	}

	return visible_entities;
}
