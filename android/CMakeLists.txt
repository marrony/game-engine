cmake_minimum_required(VERSION 2.6)

project(android)

if(NOT DEFINED ANDROID)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

if(GLEW_FOUND)
include_directories(${GLEW_INCLUDE_PATH})
endif()
endif(NOT DEFINED ANDROID)

add_library(engine-android SHARED main.cpp)

target_link_libraries(
    engine-android
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${LIBRARIES}
)
