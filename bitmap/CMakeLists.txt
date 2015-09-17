cmake_minimum_required(VERSION 3.2)
project(freelist)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")

add_library(bitmap bitmap.c)

add_executable(gentables gentables.c)
