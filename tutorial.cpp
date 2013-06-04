/*
 * tutorial.cpp
 *
 *  Created on: 27/05/2013
 *      Author: marrony
 */

#include "lua.hpp"

#include <math.h>

static int print_c(lua_State *L) {
	const char* str = lua_tostring(L, 1);
	printf("%s", str);
	return 0;
}

static int my_sin(lua_State *L) {
	lua_pushnumber(L, sin(lua_tonumber(L, 1)));
	return 1;
}

#ifndef LUA_GLOBALSINDEX
#define LUA_GLOBALSINDEX 0
#error não deveria ter acontecido
#endif

#ifndef LUA_ENVIRONINDEX
#define LUA_ENVIRONINDEX 0
#error não deveria ter acontecido
#endif

lua_State *lua_create_thread(lua_State *L) {
	lua_State *thread = lua_newthread(L);

	/**
	 * tab0 = {}
	 * tab1 = {__index = _G}
	 * setmetatable(tab0, tab1}
	 * _G = tab0
	 */
//	//lua_pushthread(thread);
//	lua_newtable(thread);                      //[tab0]
//	lua_newtable(thread);                      //[tab0, tab1]
//	lua_pushliteral(thread, "__index");        //[tab0, tab1, "__index"]
//	lua_pushvalue(thread, LUA_GLOBALSINDEX);   //[tab0, tab1, "__index", _G]
//	lua_settable(thread, -3);                  //[tab0, tab1] -- tab1.__index = _G
//	lua_setmetatable(thread, -2);              //[tab0] -- setmetatable(tab0, tab1)
//	lua_replace(thread, LUA_GLOBALSINDEX);     //[] -- _G = tab0
//	//lua_setfenv(thread, -2);

	/**
	 * tab0 = {__index = _G}
	 * setmetatable(tab0, tab0)
	 * _G = tab0
	 */
	lua_newtable(thread);                       //[tab0]
	lua_pushvalue(thread, -1);                  //[tab0, tab0]
	lua_pushliteral(thread, "__index");         //[tab0, tab0, "__index"]
	lua_pushvalue(thread, LUA_GLOBALSINDEX);    //[tab0, tab0, "__index", _G]
	lua_settable(thread, -3);                   //[tab0, tab0] -- tab0["__index"] = _G
	lua_setmetatable(thread, -2);               //[tab0] -- setmetatable(tab0, tab0)
	lua_replace(thread, LUA_GLOBALSINDEX);      //[] -- _G = tab0

	return thread;
}

void Teste_new(lua_State* L, const char* name) {
	lua_getglobal(L, "Teste");
	lua_getfield(L, -1, "new");
	lua_getglobal(L, "Teste");
	lua_newtable(L);
	lua_pushstring(L, name);
	lua_setfield(L, -2, "name");
	lua_call(L, 2, 1);
}

void DerivedTeste_new(lua_State* L, const char* name, const char* derived_attr) {
	lua_getglobal(L, "DerivedTeste");
	lua_getfield(L, -1, "new");
	lua_getglobal(L, "DerivedTeste");
	lua_newtable(L);
	lua_pushstring(L, name);
	lua_setfield(L, -2, "name");
	lua_pushstring(L, derived_attr);
	lua_setfield(L, -2, "derived_attr");
	lua_call(L, 2, 1);
}

void update(lua_State* L, const char* obj) {
	lua_getglobal(L, obj);
	lua_getmetatable(L, -1);
	lua_remove(L, -2);
	lua_getfield(L, -1, "update");
	lua_getglobal(L, obj);
	lua_call(L, 1, 0);
}

int main(int argc, char *argv[]) {
	lua_State* L = luaL_newstate();

	luaL_openlibs(L);

	lua_register(L, "print_c", print_c);

	lua_pushcfunction(L, my_sin);
	lua_setglobal(L, "my_sin");

	luaL_dofile(L, "test.lua");

	DerivedTeste_new(L, "obj0 in L", "my derived attribute");
	lua_setglobal(L, "obj0");

	lua_State* state0 = lua_create_thread(L);
	lua_State* state1 = lua_create_thread(L);

	Teste_new(state0, "obj in state0");
	lua_setglobal(state0, "obj");

	Teste_new(state1, "obj in state1");
	lua_setglobal(state1, "obj");

	update(L, "obj0");
	update(state0, "obj");
	update(state1, "obj");
	fflush(stdout);

	update(L, "obj0");
	update(state0, "obj");
	update(state1, "obj");
	fflush(stdout);

	update(L, "obj0");
	update(state0, "obj");
	update(state1, "obj");
	fflush(stdout);

	lua_close(L);

	return 0;
}
