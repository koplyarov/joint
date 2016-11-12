find_package(Java)
include(UseJava)

set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/joint.jar)

joint_generate_adapters(java ${CMAKE_CURRENT_BINARY_DIR}/Adapters.java ../idl/IBenchmarks.idl)

add_jar(benchmarks-java
	SOURCES
		Benchmarks.java
		${CMAKE_CURRENT_BINARY_DIR}/Adapters.java
    OUTPUT_DIR
		${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

add_dependencies(benchmarks-java joint-java)
