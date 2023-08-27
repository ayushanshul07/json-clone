# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(aajson_example)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(aajson_example main.cpp)
