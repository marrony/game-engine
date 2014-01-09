cmake_minimum_required(VERSION 2.6)

project(engine)

add_library(socket_static STATIC socket.cpp)
add_library(json_static STATIC json.cpp)
add_library(engine_static STATIC engine.cpp shader.cpp task.cpp entity.cpp scene_graph.cpp render.cpp)
add_library(stream_static STATIC stream.cpp)
add_library(protocol_static STATIC protocol.cpp)

if(DEFINED UNIX)
    if(DEFINED APPLE)
        set(SWAP_CHAIN_SRC ${SWAP_CHAIN_SRC} swap_chain_mac.cpp swap_chain_mac.mm)
        set(LIBRARIES ${LIBRARIES} "-framework Cocoa -framework Opengl -lobjc")
    elseif(DEFINED ANDROID)
        set(SWAP_CHAIN_SRC ${SWAP_CHAIN_SRC} swap_chain_android.cpp)
    else()
        set(SWAP_CHAIN_SRC ${SWAP_CHAIN_SRC} swap_chain_x11.cpp)
    endif()
elseif()
    set(SWAP_CHAIN_SRC ${SWAP_CHAIN_SRC} swap_chain_win32.cpp)
endif()

add_library(swap_chain_static STATIC ${SWAP_CHAIN_SRC})

if(NOT ANDROID)
add_executable(engine main.cpp)
target_link_libraries(
    engine
    engine_static
    json_static
    socket_static
    protocol_static
    swap_chain_static
    ${LUA_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${LIBRARIES}
)

#add_dependencies(engine luajit)

endif(NOT ANDROID)
