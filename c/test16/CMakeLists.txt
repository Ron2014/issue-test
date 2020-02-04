cmake_minimum_required(VERSION 3.10)

project(Static C)

add_definitions(-DINIT_ONCE)
add_executable(main main.c part.c)
