project(test-cpp)

find_package(Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

################################################################################

set(TEST_CPP_SRC
	Tests.cpp
)

add_library(tests SHARED ${TEST_CPP_SRC})
target_link_libraries(tests ${CMAKE_THREAD_LIBS_INIT})
