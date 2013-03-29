cmake_minimum_required(VERSION 2.6)

project(x11-win32)

find_package(OpenGL REQUIRED)

include_directories(library ${OPENGL_INCLUDE_DIRS})

add_executable(prog main.cpp app.cpp app_win32.cpp app_x11.cpp json.cpp)
target_link_libraries(prog ${OPENGL_LIBRARIES})

install(TARGETS prog DESTINATION bin)
