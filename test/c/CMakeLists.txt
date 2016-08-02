project(test-c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

if (MSVC)
else()
	add_definitions(-Wall)
endif()

################################################################################

set(TEST_C_SRC
	Tests.c
)

add_library(tests-c SHARED ${TEST_C_SRC})
target_link_libraries(tests-c)
