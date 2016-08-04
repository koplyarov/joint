project(joint)
cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include_directories(${CMAKE_SOURCE_DIR}/joint)

add_subdirectory(joint)
add_subdirectory(bindings/python)
add_subdirectory(bindings/cpp)
add_subdirectory(test)
