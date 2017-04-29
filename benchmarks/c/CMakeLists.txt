include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/bindings/cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (NOT MSVC)
	add_definitions(-Wall)
endif()

################################################################################

joint_generate_adapters(c Benchmarks_adapters.h ../idl/IBenchmarks.idl)

add_library(benchmarks-c SHARED Benchmarks.c OtherTranslationUnit.c Benchmarks_adapters.h)
target_link_libraries(benchmarks-c joint)

joint_make_module_package(benchmarks-component-c
	PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}/c/Benchmarks
	MANIFEST Benchmarks.jm
)

joint_add_module_content(benchmarks-component-c
	DIRECTORY .
	LIBRARIES benchmarks-c
)
