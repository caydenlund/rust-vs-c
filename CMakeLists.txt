cmake_minimum_required(VERSION 3.14)
project(rust-vs-c)

set(CMAKE_C_FLAGS_RELEASE -O3)

include_directories(./include)

add_executable(malloc-c malloc-c/main.c)

add_executable(concurrency-c concurrency-c/main.c)
