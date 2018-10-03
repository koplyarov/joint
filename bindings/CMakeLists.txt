cmake_minimum_required(VERSION 2.8)

if (POLICY CMP0058 )
    cmake_policy(SET CMP0058 OLD)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(c)
add_subdirectory(cpp)
add_subdirectory(go)
add_subdirectory(java)
add_subdirectory(native)
add_subdirectory(python)
