cmake_minimum_required(VERSION 2.6)

project(engine)

if(NOT DEFINED ANDROID)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

if(GLEW_FOUND)
include_directories(${GLEW_INCLUDE_PATH})
endif()
endif(NOT DEFINED ANDROID)

if(ANDROID)
file(GLOB NATIVE_APP_GLUE ${ANDROID_NDK}/sources/android/native_app_glue/*.c)
add_library(native_app_glue STATIC ${NATIVE_APP_GLUE})
include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
include_directories(${ANDROID_NDK}/sources/android)
set(LIBRARIES android GLESv2 log native_app_glue)
endif(ANDROID)

add_library(socket_static STATIC socket.cpp)
add_library(json_static STATIC json.cpp)
add_library(engine_static STATIC swap_chain.cpp shader.cpp task.cpp)
add_library(stream_static STATIC stream.cpp)
add_library(protocol_static STATIC protocol.cpp)

if(ANDROID)
add_library(engine SHARED engine.cpp)
else(ANDROID)
add_executable(engine engine.cpp)
endif(ANDROID)

target_link_libraries(
    engine engine_static json_static socket_static protocol_static
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${LIBRARIES}
)
