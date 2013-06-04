/*
 * scene_graph.cpp
 *
 *  Created on: 24/05/2013
 *      Author: marrony
 */

#include "scene_graph.h"

SceneGraph::SceneGraph() {
	Matrix4 m = MATRIX4_IDENTITY;

	local.push_back(m);
	world.push_back(m);
	parents.push_back(0);

	local.push_back(m);
	world.push_back(m);
	parents.push_back(0);
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

int32_t SceneGraph::create_node(int32_t parent) {
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
	} else {
		local[index] = m;
		world[index] = m;
		parents[index] = parent;
	}

	return index;
}

void SceneGraph::destroy_node(int32_t node) {
	parents[node] = 0;
}

void SceneGraph::transform_node(int32_t node, const Matrix4& m) {
	local[node] = local[node] * m;
}