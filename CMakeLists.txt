cmake_minimum_required(VERSION 3.20)
project(rasterizer-c LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Grab all .c files from src
file(GLOB SRC_FILES src/*.c)

add_executable(rasterizer-c ${SRC_FILES})

# Include raylib headers
target_include_directories(rasterizer-c PRIVATE include)

target_include_directories(rasterizer-c PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_directories(rasterizer-c PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(rasterizer-c PRIVATE raylib opengl32 gdi32 winmm)