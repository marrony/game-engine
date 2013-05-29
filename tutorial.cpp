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
//	lua_newtable(thread);                      //[tab0]
//	lua_newtable(thread);                      //[tab0, tab1]
//	lua_pushliteral(thread, "__index");        //[tab0, tab1, "__index"]
//	lua_pushvalue(thread, LUA_GLOBALSINDEX);   //[tab0, tab1, "__index", _G]
//	lua_settable(thread, -3);                  //[tab0, tab1] -- tab1.__index = _G
//	lua_setmetatable(thread, -2);              //[tab0] -- setmetatable(tab0, tab1)
//	lua_replace(thread, LUA_GLOBALSINDEX);     //[] -- _G = tab0

	/**
	 * tab0 = {__index = _G}
	 * setmetatable(tab0, tab0)
	 * _G = tab0
	 */
//	lua_newtable(thread);                       //[tab0]
//	lua_pushvalue(thread, -1);                  //[tab0, tab0]
//	lua_pushliteral(thread, "__index");         //[tab0, tab0, "__index"]
//	lua_pushvalue(thread, LUA_GLOBALSINDEX);    //[tab0, tab0, "__index", _G]
//	lua_settable(thread, -3);                   //[tab0, tab0] -- tab0["__index"] = _G
//	lua_setmetatable(thread, -2);               //[tab0] -- setmetatable(tab0, tab0)
//	lua_replace(thread, LUA_GLOBALSINDEX);      //[] -- _G = tab0

	return thread;
}

int main(int argc, char *argv[]) {
	lua_State* L = luaL_newstate();

	luaL_openlibs(L);

	lua_register(L, "print_c", print_c);

	lua_pushcfunction(L, my_sin);
	lua_setglobal(L, "my_sin");

	luaL_loadfile(L, "test.lua");
	lua_setglobal(L, "teste");

//	lua_pushstring(L, "L");
//	lua_setglobal(L, "str");

	lua_State* state0 = lua_create_thread(L);
	lua_State* state1 = lua_create_thread(L);

//	lua_pushstring(state0, "state0");
//	lua_setglobal(state0, "str");
//
//	lua_pushstring(state1, "state1");
//	lua_setglobal(state1, "str");

	lua_getglobal(state0, "teste");
	lua_getglobal(state1, "teste");

	lua_resume(state0, 0);
	lua_resume(state1, 0);
	lua_resume(state0, 0);
	lua_resume(state1, 0);
	lua_resume(state0, 0);
	lua_resume(state1, 0);

	lua_close(L);

	return 0;
}
