include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/bindings/c)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (NOT MSVC)
    add_definitions(-Wall)
endif()

################################################################################

set(IDL_FILES
    ${BENCHMARKS_IDL_DIR}/IArrayBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IBasicBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/ICastBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IEnumBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IExceptionBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IObjectBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IStructBenchmarks.idl
)

joint_generate_adapters(c Benchmarks_adapters.h ${IDL_FILES})

set(SOURCES
    Benchmarks.c
    OtherTranslationUnit.c
)

set(GENERATED_SOURCES
    Benchmarks_adapters.h
)

add_library(benchmarks-c SHARED ${SOURCES} ${GENERATED_SOURCES})
target_link_libraries(benchmarks-c ${JOINT_LIBRARIES})
source_group(generated FILES ${GENERATED_SOURCES})
source_group(idl FILES ${IDL_FILES})
auto_source_group(${SOURCES})

joint_make_module_package(benchmarks-component-c
    PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}/c/Benchmarks
    MANIFEST Benchmarks.jm
)

joint_add_module_content(benchmarks-component-c
    DIRECTORY .
    LIBRARIES benchmarks-c
)

auto_folder(benchmarks-c benchmarks-component-c)