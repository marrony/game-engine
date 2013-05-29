cmake_minimum_required(VERSION 2.6)

project(android)

file(GLOB NATIVE_APP_GLUE ${ANDROID_NDK}/sources/android/native_app_glue/*.c)
add_library(native_app_glue STATIC ${NATIVE_APP_GLUE})
include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
include_directories(${ANDROID_NDK}/sources/android)
set(LIBRARIES android EGL GLESv2 log native_app_glue)

add_library(engine-android SHARED main.cpp)

target_link_libraries(
    engine-android engine_static json_static socket_static protocol_static
    ${LUA_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${LIBRARIES}
)
