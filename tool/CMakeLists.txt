cmake_minimum_required(VERSION 2.6)

project(tool-test)

add_executable(tool main.cpp app.cpp app_win32.cpp app_x11.cpp app_mac.cpp exec.cpp)
target_link_libraries(
    tool engine_static socket_static json_static stream_static protocol_static
    ${LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
