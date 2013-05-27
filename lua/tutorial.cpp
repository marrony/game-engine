/*
 * tutorial.cpp
 *
 *  Created on: 27/05/2013
 *      Author: marrony
 */

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

static int print_c(lua_State *L) {
	const char* str = lua_tostring(L, 1);
	printf("%s", str);
	return 0;
}

int main(int argc, char *argv[]) {
	lua_State* L = luaL_newstate();

	luaL_openlibs(L);

	lua_register(L, "print_c", print_c);

	luaL_dofile(L, "test.lua");

	lua_close(L);

	return 0;
}
