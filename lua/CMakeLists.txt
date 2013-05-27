cmake_minimum_required(VERSION 2.6)

project(lua)

file(GLOB LUA_SRC src/*.c)

add_definitions(-DLUA_ANSI)
add_library(lua_static STATIC ${LUA_SRC})

include_directories(src)

add_executable(lua-tutorial tutorial.cpp)
target_link_libraries(lua-tutorial lua_static)
