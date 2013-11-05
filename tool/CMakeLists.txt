cmake_minimum_required(VERSION 2.6)

project(tool-test)

set(TOOL_SRC
    main.cpp
    app.cpp
    exec.cpp
)

if(WIN32)
    set(TOOL_SRC ${TOOL_SRC} app_win32.cpp)
endif()

if(UNIX)
    if(APPLE)
        set(TOOL_SRC ${TOOL_SRC} app_mac.cpp app_mac.m)
    else()
        set(TOOL_SRC ${TOOL_SRC} app_x11.cpp)
    endif()
endif()

add_executable(tool ${TOOL_SRC})
target_link_libraries(
    tool engine_static socket_static json_static stream_static protocol_static
    ${LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
