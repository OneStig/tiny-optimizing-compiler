cmake_minimum_required(VERSION 3.28)
project(tiny_optimizing_compiler)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(tiny_optimizing_compiler src/main.cpp
        include/Token.h
        include/FileReader.h
)
