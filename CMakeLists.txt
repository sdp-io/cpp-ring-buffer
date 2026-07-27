cmake_minimum_required(VERSION 3.16)

project(ring-buffer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ring-buffer src/main.cpp)

target_include_directories(ring-buffer PRIVATE include)

target_compile_options(ring-buffer PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wold-style-cast
    -Wshadow
)
