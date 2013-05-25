/*
 * scene_graph.h
 *
 *  Created on: 19/04/2013
 *      Author: marrony
 */

#ifndef SCENE_GRAPH_H_
#define SCENE_GRAPH_H_

#include "matrix4.h"

#include <stdint.h>

#include <vector>

class SceneGraph {
	std::vector<Matrix4> local;
	std::vector<Matrix4> world;
	std::vector<int32_t> parents;
public:
	static const int32_t ROOT = 1;

	SceneGraph();

	void update();

	Matrix4 get_local_matrix(int32_t node) const;
	Matrix4 get_world_matrix(int32_t node) const;

	int32_t create_node(int32_t parent = ROOT);
	void destroy_node(int32_t node);

	void transform_node(int32_t node, const Matrix4& m);
};

#endif /* SCENE_GRAPH_H_ */
