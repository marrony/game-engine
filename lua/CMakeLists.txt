cmake_minimum_required(VERSION 2.6)

project(lua)

file(GLOB LUA_SRC src/*.c)

add_definitions(-DLUA_ANSI)
add_library(lua_static STATIC ${LUA_SRC})

add_executable(lua-tutorial tutorial.cpp)
target_link_libraries(lua-tutorial ${LUA_LIBRARIES})
#add_dependencies(lua-tutorial luajit)
