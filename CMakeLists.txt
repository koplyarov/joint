project(joint)
cmake_minimum_required(VERSION 2.8.12)

find_package(Threads)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/joint)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

################################################################################

add_subdirectory(joint)
add_subdirectory(bindings/python)
add_subdirectory(bindings/cpp)
add_subdirectory(test)
