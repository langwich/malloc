cmake_minimum_required(VERSION 3.2)
project(malloc)

set(MODULE_NAME bytemap)

BUILD_MODULE(${MODULE_NAME} ON)
ADD_TEST_TARGET(bytemap_basic_tests ${MODULE_NAME})
ADD_TEST_TARGET(bytemap_replay ${MODULE_NAME})