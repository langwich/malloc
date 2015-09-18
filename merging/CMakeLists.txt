cmake_minimum_required(VERSION 3.2)
project(malloc)

add_library(merging merging.c)

# Each add executable could potentially need a target_link_library.
# Uncomment the next to lines to add executable target.
#add_executable(test_basic_sanity test_basic_sanity.c)
#target_link_libraries(test_basic_sanity freelist cunit lib)
