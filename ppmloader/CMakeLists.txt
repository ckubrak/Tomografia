cmake_minimum_required(VERSION 3.8)
project(ppmloader)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    ppmloader.cpp
    ppmloader.h)

add_executable(ppmloader main.cpp ${SOURCE_FILES})
add_executable(test_ORLFaces test_ORLFaces.cpp ${SOURCE_FILES})
