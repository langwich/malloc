cmake_minimum_required(VERSION 3.2)
project(malloc)

add_library(freelist freelist.c)

add_executable(test_basic_sanity test_basic_sanity.c)
add_executable(test_size_adjust test_size_adjust.c)

target_link_libraries(test_basic_sanity freelist cunit malloc)
target_link_libraries(test_size_adjust freelist cunit malloc)