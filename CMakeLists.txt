cmake_minimum_required(VERSION 3.12)
project(LFQ1)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(LFQ1 main.cpp)