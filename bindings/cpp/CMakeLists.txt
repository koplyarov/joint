project(joint-cpp)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(joint-cpp-test test/LinearizedInterfacesList.cpp)
target_link_libraries(joint-cpp-test)

# TODO: install headers
