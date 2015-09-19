cmake_minimum_required(VERSION 3.2)
project(malloc)

add_library(bitmap bitmap.c)

add_executable(bitmap_basic_tests bitmap_basic_tests.c)

target_link_libraries(bitmap_basic_tests bitmap cunit malloc)