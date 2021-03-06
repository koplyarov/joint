find_package(Java)
include(UseJava)

set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/joint.jar)

joint_generate_adapters(java ${CMAKE_CURRENT_BINARY_DIR}/Adapters.java
    ${BENCHMARKS_IDL_DIR}/IArrayBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IBasicBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/ICastBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IEnumBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IExceptionBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IObjectBenchmarks.idl
    ${BENCHMARKS_IDL_DIR}/IStructBenchmarks.idl
)

add_jar(benchmarks-java
    SOURCES
        Benchmarks.java
        ${CMAKE_CURRENT_BINARY_DIR}/Adapters.java
    INCLUDE_JARS
        ${CMAKE_BINARY_DIR}/bin/joint.jar
    OUTPUT_DIR
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

add_dependencies(benchmarks-java joint-java)

joint_make_module_package(benchmarks-component-java
    PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}/java/Benchmarks
    MANIFEST Benchmarks.jm
    DEPENDS benchmarks-java
)

joint_add_module_content(benchmarks-component-java
    DIRECTORY .
    FILES ${CMAKE_CURRENT_BINARY_DIR}/benchmarks-java.jar
)

auto_folder(benchmarks-java benchmarks-component-java)
