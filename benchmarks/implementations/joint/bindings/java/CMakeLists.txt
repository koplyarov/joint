find_package(Java)
include(UseJava)

set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/joint.jar)

joint_generate_adapters(java ${CMAKE_CURRENT_BINARY_DIR}/Adapters.java ${BENCHMARKS_IDL_DIR}/IBenchmarks.idl)

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
