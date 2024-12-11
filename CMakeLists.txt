cmake_minimum_required(VERSION 3.14)
project(commander)

set(CMAKE_C_FLAGS_RELEASE -O3)

include_directories(.)

add_executable(malloc-c
    malloc-c/main.c
)
