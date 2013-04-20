/*
 * scene_graph.h
 *
 *  Created on: 19/04/2013
 *      Author: marrony
 */

#ifndef SCENE_GRAPH_H_
#define SCENE_GRAPH_H_

#include "matrix4.h"

#include <vector>

class SceneGraph {
	std::vector<Matrix4> local;
	std::vector<Matrix4> world;
	std::vector<int> parent;
public:
	SceneGraph() {
		Matrix4 m = MATRIX4_IDENTITY;

		for(int i = 0; i < 10000; i++) {
			local.push_back(m);
			world.push_back(m);
			parent.push_back(i - 1);
		}
	}

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
