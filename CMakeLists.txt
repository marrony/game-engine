cmake_minimum_required(VERSION 2.6)

project(x11-win32)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

if(UNIX)
find_package(X11 REQUIRED)
set(LIBRARIES ${X11_LIBRARIES})
endif(UNIX)

if(WIN32)
set(LIBRARIES ws2_32)
endif(WIN32)

set(CMAKE_THREAD_PREFER_PTHREAD)
find_package(Threads REQUIRED)

add_subdirectory(tinyxml)
add_subdirectory(collada)

add_subdirectory(engine)
include_directories(engine)

add_subdirectory(tool)

#install(TARGETS prog DESTINATION bin)
