cmake_minimum_required(VERSION 2.6)

project(pas CXX)

set(CXX_FLAGS -g -Wall)
set(CMAKE_CXX_COMPILER "g++")
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(Thread_test Thread_test.cpp Thread.cpp)
target_link_libraries(Thread_test pthread)
