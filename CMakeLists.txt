project(joint)
cmake_minimum_required(VERSION 2.8.12)

find_package(Threads)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

################################################################################

add_subdirectory(core)
add_subdirectory(bindings/python)

################################################################################

include(joint.cmake)
joint_import_directories(core/idl)
joint_generate_adapters(python ${CMAKE_CURRENT_SOURCE_DIR}/test/python/TestObjects.py test/python/TestObjects.idl)
add_custom_target(test-adapters ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/python/TestObjects.py)

################################################################################


joint_generate_adapters(c++ TestObjects.hpp test/python/TestObjects.idl)
add_custom_target(test-adapters2 ALL DEPENDS TestObjects.hpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(joint-test ${CMAKE_CURRENT_SOURCE_DIR}/test/test.cpp)
target_link_libraries(joint-test joint-python)
