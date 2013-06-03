cmake_minimum_required(VERSION 2.6)

project(lua)

file(GLOB LUA_SRC src/*.c)

include_directories(src)

add_definitions(-DLUA_ANSI)
add_library(lua_static STATIC ${LUA_SRC})
