cmake_minimum_required(VERSION 3.16.3)
project(RISC_V)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-O2)
add_executable(code main.cpp)