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
#include "entity.h"
#include "render.h"
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

#include "lua.hpp"

class Engine {
	static int model_count(lua_State *L);
	static int get_node(lua_State *L);
	static int rotate_node(lua_State *L);

	lua_State* lua;
	TaskManager task_manager;
	SceneGraph scene_graph;
	EntitySystem entity_system;
	RenderSystem render_system;
	SwapChain swap_chain;

	int width;
	int height;

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

	int32_t create_model(int32_t entity, const char* mesh_name) {
		int32_t model = render_system.create_model(mesh_name);
		entity_system.add_component(entity, render_system.MODEL_TYPE, model);
		return model;
	}

	int32_t create_node(int32_t entity) {
		int32_t node = scene_graph.create_node();
		entity_system.add_component(entity, scene_graph.TYPE, node);
		return node;
	}

	int32_t create_entity(const std::string& entity_name) {
		return entity_system.create_entity(entity_name);
	}

	void transform_model(int32_t model, const Matrix4& m);

	void resize(int width, int height);

	void initialize(WindowID handle, int width, int height);
	void finalize();

	void run_one_frame();
};

#endif /* ENGINE_H_ */
