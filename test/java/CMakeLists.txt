project(test-java)

find_package(Java)
include(UseJava)

set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/joint.jar)

add_jar(tests
	SOURCES org/joint/Tests.java
    OUTPUT_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

#joint_generate_adapters(python ${CMAKE_CURRENT_SOURCE_DIR}/Tests_adapters.py
	#../idl/IBasicTests.idl
	#../idl/IObjectTests.idl
	#../idl/IEnumTests.idl
	#../idl/ILifetimeTests.idl
	#../idl/ICastTests.idl
	#../idl/IExceptionTests.idl
	#../idl/IInterfaceChecksumTests.idl
	#../idl/IStructTests.idl
	#../idl/IArrayTests.idl
#)

#add_custom_target(test-adapters-py ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Tests_adapters.py)


