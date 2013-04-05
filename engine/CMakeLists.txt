cmake_minimum_required(VERSION 2.6)

project(engine)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

if(GLEW_FOUND)
include_directories(${GLEW_INCLUDE_PATH})
endif()

add_library(socket_static STATIC socket.cpp)
add_library(json_static STATIC json.cpp)
add_library(engine_static STATIC swap_chain.cpp)

add_executable(engine engine.cpp)
target_link_libraries(engine engine_static json_static socket_static ${OPENGL_LIBRARIES} ${LIBRARIES})