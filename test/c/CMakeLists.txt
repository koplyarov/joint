project(test-c)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/bindings/cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (NOT MSVC)
	add_definitions(-Wall)
endif()

################################################################################

include(${CMAKE_SOURCE_DIR}/joint.cmake)
joint_import_directories(${CMAKE_SOURCE_DIR}/joint/idl)

joint_generate_adapters(c Tests_adapters.h
	../idl/IBasicTests.idl
	../idl/IObjectTests.idl
	../idl/ILifetimeTests.idl)

add_library(tests-c SHARED Tests.c Tests_adapters.h)
target_link_libraries(tests-c)
