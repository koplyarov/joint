set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${JNI_INCLUDE_DIRS})

include(${SWIG_USE_FILE})
set(swig_SRCS ISwigBenchmarks.i)

set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR}/bin)

set_source_files_properties(${swig_SRCS} PROPERTIES CPLUSPLUS ON)
if (MSVC)
    add_definitions(-DSWIG_PYTHON_INTERPRETER_NO_DEBUG)
    add_definitions(-DMS_NO_COREDLL)
    add_definitions(-DPy_ENABLE_SHARED)
else()
    set_source_files_properties("${CMAKE_SWIG_OUTDIR}/ISwigBenchmarksPYTHON_wrap.cxx" PROPERTIES COMPILE_FLAGS "-Wno-error -Wno-maybe-uninitialized")
    set_source_files_properties("${CMAKE_SWIG_OUTDIR}/ISwigBenchmarksJAVA_wrap.cxx" PROPERTIES COMPILE_FLAGS "-Wno-error -Wno-strict-aliasing")
    set_source_files_properties("ISwigBenchmarks.cpp" PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

set(SWIG_MODULE_swig_benchmarks_wrappers_py_EXTRA_DEPS ISwigBenchmarks.hpp ISwigBenchmarks.cpp)
set(SWIG_MODULE_swig_benchmarks_wrappers_java_EXTRA_DEPS ISwigBenchmarks.hpp ISwigBenchmarks.cpp)

add_library(swig_benchmarks_native ISwigBenchmarks.cpp)

#set(CMAKE_SWIG_FLAGS "-threads")
swig_add_module(swig_benchmarks_wrappers_py python ${swig_SRCS})
swig_link_libraries(swig_benchmarks_wrappers_py swig_benchmarks_native ${PYTHON_LIBRARIES})

set(CMAKE_SWIG_FLAGS -package SB)
swig_add_module(swig_benchmarks_wrappers_java java ${swig_SRCS})
swig_link_libraries(swig_benchmarks_wrappers_java swig_benchmarks_native ${JNI_LIBRARIES})

add_jar(benchmarks-java-swig
    SOURCES
        SwigBenchmarks.java
        ${CMAKE_SWIG_OUTDIR}/IBasicBenchmarks.java
        ${CMAKE_SWIG_OUTDIR}/IBasicInvokable.java
        ${CMAKE_SWIG_OUTDIR}/swig_benchmarks_wrappers_java.java
        ${CMAKE_SWIG_OUTDIR}/swig_benchmarks_wrappers_javaJNI.java
    OUTPUT_DIR
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)
add_dependencies(benchmarks-java-swig swig_benchmarks_wrappers_java)
