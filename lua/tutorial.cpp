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

int main(int argc, char *argv[]) {
	lua_State* L = luaL_newstate();

	luaL_openlibs(L);

	lua_register(L, "print_c", print_c);

	lua_pushcfunction(L, my_sin);
	lua_setglobal(L, "my_sin");

	luaL_dofile(L, "test.lua");

	lua_close(L);

	return 0;
}
