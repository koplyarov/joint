project(test-cpp)

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

################################################################################

joint_generate_adapters(c++ Tests_adapters.hpp
	${IDL_DIR}/IStarterTests.idl
	${IDL_DIR}/IBasicTests.idl
	${IDL_DIR}/IEnumTests.idl
	${IDL_DIR}/IObjectTests.idl
	${IDL_DIR}/ILifetimeTests.idl
	${IDL_DIR}/ICastTests.idl
	${IDL_DIR}/IExceptionTests.idl
	${IDL_DIR}/IInterfaceChecksumTests.idl
	${IDL_DIR}/IStructTests.idl
	${IDL_DIR}/IArrayTests.idl

)

add_library(tests-cpp SHARED Tests.cpp Tests_adapters.hpp)
target_link_libraries(tests-cpp ${JOINT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

joint_make_module_package(tests-component-cpp
	PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}/cpp/Tests
	MANIFEST Tests.jm
)

joint_add_module_content(tests-component-cpp
	DIRECTORY .
	LIBRARIES tests-cpp
)
