find_package(Threads)

include_directories(${JOINT_CPP_INCLUDE_DIRECTORIES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
    add_definitions(-Wall)
    add_definitions(-std=c++11)
endif()

set(IDL_FILES
    ${BENCHMARKS_IDL_DIR}/IArrayBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IBasicBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/ICastBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IEnumBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IExceptionBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IObjectBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IStructBenchmarks.idl
)

joint_generate_adapters(c++ Benchmarks_adapters.hpp ${IDL_FILES})

################################################################################

set(SOURCES
    Benchmarks.cpp
    OtherTranslationUnit.cpp
)

set(GENERATED_SOURCES
    Benchmarks_adapters.hpp
)

add_library(benchmarks-cpp SHARED ${SOURCES} ${GENERATED_SOURCES})
target_link_libraries(benchmarks-cpp ${JOINT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
source_group(idl FILES ${IDL_FILES})
source_group(generated FILES ${GENERATED_SOURCES})
source_group(idl FILES ${IDL_FILES})
auto_source_group(${SOURCES})

joint_make_module_package(benchmarks-component-cpp
    PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}/cpp/Benchmarks
    MANIFEST except/Benchmarks.jm
)

joint_add_module_content(benchmarks-component-cpp
    DIRECTORY .
    LIBRARIES benchmarks-cpp
)

################################################################################

if (MSVC)
    if ("${CMAKE_GENERATOR}" STREQUAL "Visual Studio 12 2013")
        set(UNCAUGHT_EXCEPTION_WORKAROUND "\"/D__uncaught_exception=[]{return false;}\"")
    else()
        set(UNCAUGHT_EXCEPTION_WORKAROUND "")
    endif()

    add_definitions(${UNCAUGHT_EXCEPTION_WORKAROUND})
    add_compile_options(/D_HAS_EXCEPTIONS=0)
else()
    add_compile_options(-fno-exceptions -fno-rtti)
endif()

add_library(benchmarks-cpp-noexcept SHARED Benchmarks.cpp OtherTranslationUnit.cpp Benchmarks_adapters.hpp)
target_link_libraries(benchmarks-cpp-noexcept ${JOINT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

joint_make_module_package(benchmarks-component-cpp-noexcept
    PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}/cpp_ne/Benchmarks
    MANIFEST noexcept/Benchmarks.jm
)

joint_add_module_content(benchmarks-component-cpp-noexcept
    DIRECTORY .
    LIBRARIES benchmarks-cpp-noexcept
)

auto_folder(benchmarks-cpp benchmarks-component-cpp benchmarks-cpp-noexcept benchmarks-component-cpp-noexcept)

