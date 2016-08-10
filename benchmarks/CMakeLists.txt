project(joint-benchmarks)

add_subdirectory(core)

find_package(SWIG)
find_package(PythonLibs)

if (BENCHMARKS_DEPENDENCIES_RESOLVED AND SWIG_FOUND AND PythonLibs_FOUND)
	if (MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
		add_definitions(-DSWIG_PYTHON_INTERPRETER_NO_DEBUG)
		add_definitions(-DMS_NO_COREDLL)
		add_definitions(-DPy_ENABLE_SHARED)
	else()
		add_definitions(-Wall)
		add_definitions(-std=c++11)
	endif()

	################################################################################

	include(${SWIG_USE_FILE})
	set(swig_SRCS swig/ISwigBenchmarks.i)

	#set(CMAKE_SWIG_FLAGS "-threads")
	set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR}/bin)

	set_source_files_properties(${swig_SRCS} PROPERTIES CPLUSPLUS ON)
	if (NOT MSVC)
		set_source_files_properties("bin/hidePYTHON_wrap.cxx" PROPERTIES COMPILE_FLAGS "-Wno-maybe-uninitialized")
		set_source_files_properties("swig/ISwigBenchmarks.cpp" PROPERTIES COMPILE_FLAGS "-fPIC")
	endif()

	set(SWIG_MODULE_swig_benchmarks_wrappers_EXTRA_DEPS swig/ISwigBenchmarks.hpp swig/ISwigBenchmarks.cpp)

	add_library(swig_benchmarks_native swig/ISwigBenchmarks.cpp)
	swig_add_module(swig_benchmarks_wrappers python ${swig_SRCS})
	swig_link_libraries(swig_benchmarks_wrappers swig_benchmarks_native ${PYTHON_LIBRARIES})

	################################################################################

	find_package(JointGen)

	add_subdirectory(python)
	add_subdirectory(cpp)
	add_subdirectory(c)

	include_directories(${PYTHON_INCLUDE_DIRS})

	include_directories(${CMAKE_CURRENT_BINARY_DIR})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core)
	include_directories(${CMAKE_SOURCE_DIR})
	include_directories(${CMAKE_SOURCE_DIR}/bindings/cpp)

	joint_generate_adapters(c++ Benchmarks_adapters.hpp idl/IBenchmarks.idl)

	if (MSVC)
		set(PLATFORM_LIBS ole32 oleaut32)
	else()
		set(PLATFORM_LIBS rt)
	endif()

	add_executable(joint-benchmarks Benchmarks_adapters.hpp main.cpp)
	target_link_libraries(joint-benchmarks swig_benchmarks_native benchmarks joint-loader ${PLATFORM_LIBS})
endif()
