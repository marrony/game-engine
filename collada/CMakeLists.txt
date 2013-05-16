cmake_minimum_required(VERSION 2.6)
project(Collada CXX C)

file(GLOB COLLADA_SRC *.cpp)

include_directories(../tinyxml)
include_directories(../engine)

add_library(collada_static ${COLLADA_SRC})

add_executable(collada-compiler 
    collada_compiler.cpp
    CreateGeometry.cpp)
target_link_libraries(collada-compiler collada_static tinyxml_static)