cmake_minimum_required(VERSION 2.6)

project(engine)

if(NOT ANDROID)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

if(GLEW_FOUND)
include_directories(${GLEW_INCLUDE_PATH})
endif(GLEW_FOUND)

else(NOT ANDROID)
include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
include_directories(${ANDROID_NDK}/sources/android)
endif(NOT ANDROID)

add_library(socket_static STATIC socket.cpp)
add_library(json_static STATIC json.cpp)
add_library(engine_static STATIC engine.cpp swap_chain.cpp shader.cpp task.cpp scene_graph.cpp)
add_library(stream_static STATIC stream.cpp)
add_library(protocol_static STATIC protocol.cpp)

if(NOT ANDROID)
add_executable(engine main.cpp)
target_link_libraries(
    engine engine_static json_static socket_static protocol_static
    ${LUA_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${LIBRARIES}
)

add_dependencies(engine luajit)

endif(NOT ANDROID)
