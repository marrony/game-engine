cmake_minimum_required(VERSION 2.6)

project(x11-win32)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
if(GLEW_FOUND)
include_directories(${GLEW_INCLUDE_PATH})
endif()

include_directories(engine)

add_library(eng STATIC engine/app.cpp engine/app_win32.cpp engine/app_x11.cpp engine/json.cpp)

add_executable(engine engine/engine.cpp)
target_link_libraries(engine eng ${OPENGL_LIBRARIES})

add_executable(tool tool/main.cpp)
target_link_libraries(tool eng ${OPENGL_LIBRARIES})

#install(TARGETS prog DESTINATION bin)
