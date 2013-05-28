cmake_minimum_required(VERSION 2.6)

project(x11-win32)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

if(UNIX AND NOT DEFINED ANDROID)
find_package(X11 REQUIRED)
set(LIBRARIES ${X11_LIBRARIES})
endif(UNIX AND NOT DEFINED ANDROID)

if(WIN32)
set(LIBRARIES ws2_32)
endif(WIN32)

if(NOT DEFINED ANDROID)
set(CMAKE_THREAD_PREFER_PTHREAD)
find_package(Threads REQUIRED)
endif(NOT DEFINED ANDROID)

add_subdirectory(tinyxml)

if(NOT ANDROID)
add_subdirectory(collada)
endif(NOT ANDROID)

include_directories(LuaJIT/src)

add_custom_target(
    luajit
    COMMAND make CCDEBUG=-g
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/LuaJIT
)

set(LUAJIT_LIBRARIES ${CMAKE_SOURCE_DIR}/LuaJIT/src/libluajit.a dl)

add_subdirectory(engine)
include_directories(engine)

if(NOT ANDROID)
add_subdirectory(tool)
endif(NOT ANDROID)

if(ANDROID)
add_subdirectory(android)
endif(ANDROID)

add_subdirectory(lua)

file(GLOB solver_srcs solver/*.cpp)

#install(TARGETS prog DESTINATION bin)
add_executable(solver ${solver_srcs})