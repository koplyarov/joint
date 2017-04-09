project(joint-test)

add_definitions(-D_GLIBCXX_DEBUG)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(JOINT_MODULE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(JointGen)

add_subdirectory(core)

add_subdirectory(c)
add_subdirectory(cpp)
add_subdirectory(python)
add_subdirectory(java)

find_package(Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/bindings/cpp)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

################################################################################

joint_generate_adapters(c++ Tests_adapters.hpp
	idl/IStarterTests.idl
	idl/IBasicTests.idl
	idl/IObjectTests.idl
	idl/ILifetimeTests.idl
	idl/ICastTests.idl
	idl/IExceptionTests.idl
	idl/IEnumTests.idl
	idl/IStructTests.idl
	IInterfaceChecksumTests.idl
	idl/IArrayTests.idl
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(joint-test Tests_adapters.hpp test.cpp)
target_link_libraries(joint-test test-core joint-loader)
