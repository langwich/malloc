cmake_minimum_required(VERSION 3.3)
project(malloc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")

# add a build target for the module
# use debug mode switch to turn on/off debug mode
function(BUILD_MODULE _MODULE_NAME _DEBUG_MODE_SWITCH)
	add_library(${_MODULE_NAME} ${_MODULE_NAME}.c)
	target_link_libraries(${_MODULE_NAME} cunit malloc)

	if(${_DEBUG_MODE_SWITCH})
		set_target_properties(${MODULE_NAME} PROPERTIES
				COMPILE_FLAGS -DDEBUG)
	endif()
endfunction(BUILD_MODULE)

# add a test target to the module
# the module should at least have cunit built-in.
function(ADD_TEST_TARGET _TEST_NAME _MODULE_NAME)
	add_executable(${_TEST_NAME} ${_TEST_NAME}.c)
	target_link_libraries(${_TEST_NAME} ${_MODULE_NAME})
endfunction(ADD_TEST_TARGET)

link_directories(bitmap freelist cunit lib)
include_directories(bitmap freelist cunit lib)

add_subdirectory(freelist)
add_subdirectory(bitmap)
add_subdirectory(cunit)
add_subdirectory(merging)
add_subdirectory(binning)
add_subdirectory(lib)
