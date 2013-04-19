/*
 * scene_graph.h
 *
 *  Created on: 19/04/2013
 *      Author: marrony
 */

#ifndef SCENE_GRAPH_H_
#define SCENE_GRAPH_H_

#include <vector>

struct Matrix4 {
	Matrix4 operator*(const Matrix4& other) const {
		return Matrix4();
	}
};

class SceneGraph {
	std::vector<Matrix4> local;
	std::vector<Matrix4> world;
	std::vector<int> parent;
public:
	void update() {
		for(size_t i = 0; i < local.size(); i++) {
			int p = parent[i];

			if(p == -1)
				world[i] = local[i];
			else
				world[i] = world[p] * local[i];
		}
	}
};

#endif /* SCENE_GRAPH_H_ */
