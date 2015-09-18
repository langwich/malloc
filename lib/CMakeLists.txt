cmake_minimum_required(VERSION 3.2)
project(malloc)

set(SOURCE morecore.c)
add_library(malloc ${SOURCE})