cmake_minimum_required(VERSION 3.14)
project(rust-vs-c)

set(CMAKE_C_FLAGS_RELEASE "-O3 -march=native -mtune=native -flto")

add_executable(concurrency-c concurrency-c/main.c)
add_executable(malloc-c malloc-c/main.c)
add_executable(matrix-c matrix-c/main.c)
add_executable(sort-c sort-c/main.c)
