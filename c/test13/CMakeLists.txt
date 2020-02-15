
cmake_minimum_required(VERSION 3.10)

project(CrossDepend C)

set(a_sources
    liba/filea.h
    liba/filea.c)

set(b_sources
    libb/fileb.h
    libb/fileb.c)

add_library(liba ${a_sources})
target_include_directories(liba PUBLIC libb)
target_link_libraries(liba libb)

add_library(libb ${b_sources})
target_include_directories(libb PUBLIC liba)
target_link_libraries(libb liba)

add_executable(test main.c)
target_include_directories(test PUBLIC liba)
target_link_libraries(test liba)