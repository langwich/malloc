cmake_minimum_required(VERSION 3.3)
project(malloc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")

link_directories(bitmap freelist cunit)
include_directories(bitmap freelist cunit)

add_subdirectory(freelist)
add_subdirectory(bitmap)
add_subdirectory(cunit)
