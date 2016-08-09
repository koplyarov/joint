project(joint-benchmarks)

add_subdirectory(core)

if (BENCHMARKS_DEPENDENCIES_RESOLVED)
	find_package(JointGen)

	add_subdirectory(python)
	add_subdirectory(cpp)
	add_subdirectory(c)

	include_directories(${CMAKE_CURRENT_BINARY_DIR})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core)
	include_directories(${CMAKE_SOURCE_DIR})
	include_directories(${CMAKE_SOURCE_DIR}/bindings/cpp)

	if (MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
	else()
		add_definitions(-Wall)
		add_definitions(-std=c++11)
	endif()

	joint_generate_adapters(c++ Benchmarks_adapters.hpp idl/IBenchmarks.idl)

	if (MSVC)
		set(PLATFORM_LIBS ole32 oleaut32)
	else()
		set(PLATFORM_LIBS rt)
	endif()

	add_executable(joint-benchmarks Benchmarks_adapters.hpp main.cpp)
	target_link_libraries(joint-benchmarks benchmarks joint-loader ${PLATFORM_LIBS})
endif()
