cmake_minimum_required(VERSION 3.2)
project(malloc)

set(SOURCE morecore.c bitset.c)
add_library(malloc ${SOURCE})

add_executable(bitset_basic_tests bitset_basic_tests.c)
target_link_libraries(bitset_basic_tests cunit malloc)