cmake_minimum_required(VERSION 3.5)
project(TP3-Metodos)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -ggdb3 -g -O0")
set(SOURCE_FILES
        ppmloader/ppmloader.cpp
        ppmloader/ppmloader.h
	src/main.cpp        
	src/sparse.cpp
	src/sparse.h
	src/rayos.hpp
	src/misc.h
	src/misc.cpp
	src/ruido.h
	src/ruido.cpp
        )

add_executable(tp3 ${SOURCE_FILES})
