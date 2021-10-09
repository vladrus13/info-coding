cmake_minimum_required(VERSION 3.20)
project(info_coding)

set(CMAKE_CXX_STANDARD 17)

add_executable(info_coding main.cpp polar/PolarCoder.cpp polar/PolarCoder.h)
