cmake_minimum_required(VERSION 3.2)
project(freelist)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    freelist.c
    test_size_adjust.c
    morecore.c
    test_basic_sanity.c
    test_size_adjust.c)

add_executable(freelist ${SOURCE_FILES})