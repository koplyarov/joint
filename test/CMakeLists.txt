project(joint-test)

find_package(Threads)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/core)
include_directories(${CMAKE_SOURCE_DIR}/bindings/cpp)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()


set(TEST_CORE_SRC
	core/TestContext.cpp
	core/TestEngine.cpp
	core/ScopedTest.cpp
	core/utils/Logger.cpp
	core/utils/TypeInfo.cpp
)

add_library(test-core ${TEST_CORE_SRC})
target_link_libraries(test-core ${CMAKE_THREAD_LIBS_INIT})


################################################################################


include(${CMAKE_SOURCE_DIR}/joint.cmake)
joint_import_directories(${CMAKE_SOURCE_DIR}/joint/idl)

joint_generate_adapters(python ${CMAKE_CURRENT_SOURCE_DIR}/python/Tests_adapters.py idl/Tests.idl)
add_custom_target(test-adapters ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/python/Tests_adapters.py)

joint_generate_adapters(c++ Tests_adapters.hpp idl/Tests.idl)
add_custom_target(test-adapters2 ALL DEPENDS Tests_adapters.hpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(joint-test test.cpp)
target_link_libraries(joint-test test-core joint-loader)
