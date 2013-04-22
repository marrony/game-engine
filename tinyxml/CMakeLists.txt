cmake_minimum_required(VERSION 2.6)
project(TinyXml CXX C)

#sources
set(TINYXML_SRC
	tinystr.cpp
	tinyxml.cpp
	tinyxmlerror.cpp
	tinyxmlparser.cpp
)

add_library(tinyxml_static ${TINYXML_SRC})
