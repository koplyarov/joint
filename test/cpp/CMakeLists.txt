project(test-cpp)

find_package(Threads)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/bindings/cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

################################################################################

include(${CMAKE_SOURCE_DIR}/joint-gen/joint.cmake)

joint_generate_adapters(c++ Tests_adapters.hpp
	../idl/IBasicTests.idl
	../idl/IObjectTests.idl
	../idl/ILifetimeTests.idl)

add_library(tests-cpp SHARED Tests.cpp Tests_adapters.hpp)
target_link_libraries(tests-cpp ${CMAKE_THREAD_LIBS_INIT})
