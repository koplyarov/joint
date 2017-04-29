project(test-java)

find_package(Java)
include(UseJava)

set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/joint.jar)

add_jar(tests
	SOURCES
		Tests.java
		${CMAKE_CURRENT_BINARY_DIR}/Adapters.java
	INCLUDE_JARS
		${CMAKE_BINARY_DIR}/bin/joint.jar
    OUTPUT_DIR
		${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

#add_dependencies(tests joint-java)

joint_generate_adapters(java ${CMAKE_CURRENT_BINARY_DIR}/Adapters.java
	${IDL_DIR}/IBasicTests.idl
	${IDL_DIR}/IEnumTests.idl
	${IDL_DIR}/IObjectTests.idl
	${IDL_DIR}/ICastTests.idl
	${IDL_DIR}/IExceptionTests.idl
	${IDL_DIR}/IStructTests.idl
	${IDL_DIR}/IArrayTests.idl
)

joint_make_module_package(tests-component-java
	PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}/java/Tests
	MANIFEST Tests.jm
	DEPENDS tests
)

joint_add_module_content(tests-component-java
	DIRECTORY .
	FILES ${CMAKE_CURRENT_BINARY_DIR}/tests.jar
)
