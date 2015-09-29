cmake_minimum_required(VERSION 3.2)
project(malloc)

set(MODULE_NAME binning)

BUILD_MODULE(${MODULE_NAME} ON)

ADD_TEST_TARGET(binning_basic_test ${MODULE_NAME})