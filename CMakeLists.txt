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

add_subdirectory(engine)
include_directories(engine)

add_executable(tool tool/main.cpp tool/app.cpp tool/app_win32.cpp tool/app_x11.cpp tool/exec.cpp)
target_link_libraries(
    tool engine_static socket_static json_static stream_static protocol_static
    ${LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

#install(TARGETS prog DESTINATION bin)
