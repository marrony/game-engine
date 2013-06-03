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

include_directories(lua/src)
add_subdirectory(lua)
set(LUA_LIBRARIES lua_static)

#if(ANDROID OR WIN32)
#message("====================== ${ANDROID_TOOLCHAIN_ROOT}/bin/${ANDROID_TOOLCHAIN_MACHINE_NAME}-")
#message("====================== ANDROID_TOOLCHAIN_MACHINE_NAME ${ANDROID_TOOLCHAIN_MACHINE_NAME}")
#message("====================== ANDROID_TOOLCHAIN_ROOT ${ANDROID_TOOLCHAIN_ROOT}")
#message("====================== ANDROID_GCC_TOOLCHAIN_NAME ${ANDROID_GCC_TOOLCHAIN_NAME}")
#message("====================== ANDROID_NDK ${ANDROID_NDK}")
#message("====================== ANDROID_SYSROOT ${ANDROID_SYSROOT}")
#message("====================== ANDROID_NDK_TOOLCHAINS_SUBPATH ${ANDROID_NDK_TOOLCHAINS_SUBPATH}")
#message("====================== ANDROID_NDK_TOOLCHAINS_PATH ${ANDROID_NDK_TOOLCHAINS_PATH}")
#message("====================== ANDROID_TOOLCHAIN_NAME ${ANDROID_TOOLCHAIN_NAME}")
#message("====================== ANDROID_CXX_FLAGS ${ANDROID_CXX_FLAGS}")
#set(NDKP "${ANDROID_TOOLCHAIN_ROOT}/bin/${ANDROID_TOOLCHAIN_MACHINE_NAME}-")
#set(NDKF "--sysroot=${ANDROID_SYSROOT}")
#set(LUAJIT_BUILDFLAGS "${LUAJIT_BUILDFLAGS} HOST_CC='gcc -m32' CROSS=${NDKP} TARGET_FLAGS='${NDKF}'")
#else(ANDROID OR WIN32)
#include_directories(LuaJIT/src)
#
#set(LUAJIT_BUILDFLAGS "BUILDMODE=static CCDEBUG=-g")
#
#add_custom_target(
#    luajit
#    COMMAND make ${LUAJIT_BUILDFLAGS}
#    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/LuaJIT
#)
#
#set(LUA_LIBRARIES ${CMAKE_SOURCE_DIR}/LuaJIT/src/libluajit.a)
#
#if(NOT WIN32)
#set(LUA_LIBRARIES ${LUA_LIBRARIES} dl)
#endif(NOT WIN32)
#endif(ANDROID OR WIN32)

add_subdirectory(engine)
include_directories(engine)

if(NOT ANDROID)
add_subdirectory(tool)
endif(NOT ANDROID)

if(ANDROID)
add_subdirectory(android)
endif(ANDROID)

file(GLOB solver_srcs solver/*.cpp)

#install(TARGETS prog DESTINATION bin)
add_executable(solver ${solver_srcs})

add_executable(lua-tutorial tutorial.cpp)
target_link_libraries(lua-tutorial ${LUA_LIBRARIES})
#add_dependencies(lua-tutorial luajit)
