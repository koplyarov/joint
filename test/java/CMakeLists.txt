project(test-java)

find_package(Java)
include(UseJava)

set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/joint.jar)

add_jar(tests
	SOURCES
		Tests.java
		${CMAKE_CURRENT_BINARY_DIR}/Adapters.java
    OUTPUT_DIR
		${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

add_dependencies(tests joint-java)

joint_generate_adapters(java ${CMAKE_CURRENT_BINARY_DIR}/Adapters.java
	../idl/IBasicTests.idl
	../idl/IObjectTests.idl
	../idl/IEnumTests.idl
)


