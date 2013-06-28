/*
 * engine.cpp
 *
 *  Created on: 01/04/2013
 *      Author: marrony
 */

#include "engine.h"
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
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

void Engine::transform_model(int32_t entity, const Matrix4& m) {
	//if(entity < 0 || entity >= models.size()) return;

	int32_t node = entity_system.get_component(entity, scene_graph.TYPE);

	scene_graph.transform_node(node, m);
}

void Engine::resize(int width, int height) {
	render_system.resize(width, height);
}

void Engine::render() {
	render_system.render();
}

WorkItem Engine::render_task() {
	WorkItem item = {render_function, this};
	return item;
}

void Engine::update() {
	swap_chain.process_events();

//	lua_getglobal(lua, "update");
//	lua_call(lua, 0, 0);

	scene_graph.update();

	render_system.collect_render_commands(scene_graph);
}

WorkItem Engine::update_task() {
	WorkItem item = {update_function, this};
	return item;
}

Engine::Engine() {
	lua = NULL;

	width = 0;
	height = 0;
}

static void set_engine(lua_State* lua, Engine* engine) {
	lua_pushstring(lua, "engine_pointer");
	lua_pushlightuserdata(lua, engine);
	lua_settable(lua, LUA_REGISTRYINDEX);
}

static Engine* get_engine(lua_State* lua) {
	lua_pushstring(lua, "engine_pointer");
	lua_gettable(lua, LUA_REGISTRYINDEX);
	return (Engine*)lua_topointer(lua, -1);
}

int Engine::model_count(lua_State *L) {
	Engine* engine = get_engine(L);
	//lua_pushinteger(L, engine->models.size());
	return 1;
}

int Engine::get_node(lua_State *L) {
	int model = luaL_checkinteger(L, 1);

	Engine* engine = get_engine(L);
	//lua_pushinteger(L, engine->models[model-1]->node);
	return 1;
}

int Engine::rotate_node(lua_State *L) {
	Vector3 axis[] = {
			{1, 0, 0},
			{0, 0, 1},
			{0, 1, 0},
	};

	int node = luaL_checkinteger(L, 1);
	int axis_idx = luaL_checkinteger(L, 2);
	double ang = luaL_checknumber(L, 3);

	Engine* engine = get_engine(L);

	Matrix4 rotationMatrix = Matrix4::rotationMatrix(Quaternion::make(axis[axis_idx], ang));
	engine->scene_graph.transform_node(node, rotationMatrix);

	return 0;
}

void Engine::initialize(WindowID handle, int width, int height) {
	lua = luaL_newstate();
	luaL_openlibs(lua);

	set_engine(lua, this);

	lua_register(lua, "model_count", model_count);
	lua_register(lua, "get_node", get_node);
	lua_register(lua, "rotate_node", rotate_node);

	if(luaL_dofile(lua, "update.lua") != 0)
		printf("erro");

	this->width = width;
	this->height = height;

	swap_chain.create(handle, width, height);

	scene_graph.initialize(entity_system);
	render_system.initialize(entity_system, &swap_chain);

	task_manager.initialize(32);

#ifdef ANDROID
	this->width = swap_chain.width;
	this->height = swap_chain.height;
#endif

	render_system.resize(width, height);
}

void Engine::run_one_frame() {
	TaskId update = task_manager.add(update_task(), INVALID_ID, pthread_self());
	TaskId render = task_manager.add(render_task(), update, pthread_self());

	task_manager.wait(update);
}

void Engine::finalize() {
	task_manager.finalize();

	swap_chain.destroy();

	lua_close(lua);

	fprintf(stderr, "finish engine\n");
	fflush(stderr);
}
