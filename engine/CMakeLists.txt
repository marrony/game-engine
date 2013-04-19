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
add_library(engine_static STATIC swap_chain.cpp task.cpp)
add_library(stream_static STATIC stream.cpp)
add_library(protocol_static STATIC protocol.cpp)

add_executable(engine engine.cpp)
target_link_libraries(
    engine engine_static json_static socket_static protocol_static
    ${OPENGL_LIBRARIES} ${LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
