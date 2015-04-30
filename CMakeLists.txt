cmake_minimum_required(VERSION 3.2)
project(image_to_sketch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES source/main.cpp)

add_executable(image_to_sketch ${SOURCE_FILES})