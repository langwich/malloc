cmake_minimum_required(VERSION 3.2)
project(malloc)

set(MODULE_NAME bitmap)

BUILD_MODULE(${MODULE_NAME} ON)

ADD_TEST_TARGET(bitmap_basic_tests ${MODULE_NAME})
ADD_TEST_TARGET(bitmap_replay ${MODULE_NAME})