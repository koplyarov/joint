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

joint_generate_adapters(c++ Benchmarks_adapters.hpp ${BENCHMARKS_IDL_DIR}/IBenchmarks.idl)

add_library(benchmarks-cpp SHARED Benchmarks.cpp OtherTranslationUnit.cpp Benchmarks_adapters.hpp)
target_link_libraries(benchmarks-cpp joint ${CMAKE_THREAD_LIBS_INIT})

joint_make_module_package(benchmarks-component-cpp
	PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}/cpp/Benchmarks
	MANIFEST Benchmarks.jm
)

joint_add_module_content(benchmarks-component-cpp
	DIRECTORY .
	LIBRARIES benchmarks-cpp
)
