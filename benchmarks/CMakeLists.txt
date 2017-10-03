project(joint-benchmarks)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(JOINT_MODULE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(core)

find_package(SWIG)
find_package(PythonLibs)
find_package(JNI)
find_package(Java)

message (STATUS "BENCHMARKS_DEPENDENCIES_RESOLVED: ${BENCHMARKS_DEPENDENCIES_RESOLVED}")
message (STATUS "SWIG_FOUND: ${SWIG_FOUND}")
message (STATUS "PYTHONLIBS_FOUND: ${PYTHONLIBS_FOUND}")
message (STATUS "JNI_FOUND: ${JNI_FOUND}")
message (STATUS "JAVA_FOUND: ${JAVA_FOUND}")

if (BENCHMARKS_DEPENDENCIES_RESOLVED AND SWIG_FOUND AND PYTHONLIBS_FOUND AND JNI_FOUND AND JAVA_FOUND)
	find_package(Joint)

	include(UseJava)

	if (MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
	else()
		add_definitions(-Wall)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	endif()

	add_subdirectory(implementations)

	include_directories(${PYTHON_INCLUDE_DIRS})
	include_directories(${JNI_INCLUDE_DIRS})

	include_directories(${CMAKE_CURRENT_BINARY_DIR})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core)
	include_directories(${JOINT_CPP_INCLUDE_DIRECTORIES})

	joint_generate_adapters(c++ Benchmarks_adapters.hpp
		implementations/joint/idl/IBasicBenchmarks.idl
		implementations/joint/idl/ICastBenchmarks.idl
		implementations/joint/idl/IEnumBenchmarks.idl
		implementations/joint/idl/IExceptionBenchmarks.idl
		implementations/joint/idl/IObjectBenchmarks.idl
		implementations/joint/idl/IStructBenchmarks.idl
	)

	if (MSVC)
		set(PLATFORM_LIBS ole32 oleaut32)
	else()
		set(PLATFORM_LIBS rt)
	endif()

	add_executable(joint-benchmarks Benchmarks_adapters.hpp main.cpp)
	target_link_libraries(joint-benchmarks swig_benchmarks_native benchmarks ${JOINT_LIBRARIES} ${PLATFORM_LIBS})
endif()
