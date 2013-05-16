cmake_minimum_required(VERSION 2.6)
project(TinyXml CXX C)

file(GLOB TINYXML_SRC *.cpp)

add_library(tinyxml_static ${TINYXML_SRC})
