cmake_minimum_required(VERSION 2.6)

project(x11-win32)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

if(UNIX)
find_package(X11 REQUIRED)
endif(UNIX)

add_subdirectory(engine)
include_directories(engine)

add_executable(tool tool/main.cpp tool/app.cpp tool/app_win32.cpp tool/app_x11.cpp tool/exec.cpp)
target_link_libraries(tool engine_static ${X11_LIBRARIES})

#install(TARGETS prog DESTINATION bin)
