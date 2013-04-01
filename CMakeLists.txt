cmake_minimum_required(VERSION 2.6)

project(x11-win32)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_PATH})

add_executable(prog main.cpp app.cpp app_win32.cpp app_x11.cpp json.cpp)
target_link_libraries(prog ${OPENGL_LIBRARIES})

install(TARGETS prog DESTINATION bin)
