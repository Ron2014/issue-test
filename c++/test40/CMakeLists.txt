cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Main VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main test.cc)
