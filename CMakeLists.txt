cmake_minimum_required(VERSION 3.3)
project(malloc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")

link_directories(bitmap freelist cunit lib)
include_directories(bitmap freelist cunit lib)

add_subdirectory(freelist)
add_subdirectory(bitmap)
add_subdirectory(cunit)
add_subdirectory(merging)
add_subdirectory(binning)
add_subdirectory(lib)
