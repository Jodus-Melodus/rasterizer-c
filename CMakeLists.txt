cmake_minimum_required(VERSION 3.20)
project(rasterizer-c LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Grab all .c files from src
file(GLOB SRC_FILES src/*.c)

add_executable(rasterizer-c ${SRC_FILES})
target_include_directories(rasterizer-c PRIVATE include)
