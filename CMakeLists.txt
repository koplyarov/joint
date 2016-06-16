project(joint)
cmake_minimum_required(VERSION 2.8.12)

find_package(Threads)
find_package(PythonLibs REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

################################################################################

set(JOINT_CORE_SRC
	core/joint/Joint.cpp
	core/joint/core/JointCore.cpp
)

add_library(joint-core SHARED ${JOINT_CORE_SRC})
target_link_libraries(joint-core ${CMAKE_THREAD_LIBS_INIT})

################################################################################

set(JOINT_PYTHON_BINDING_SRC
	bindings/python/PythonBinding.cpp
	bindings/python/PythonModule.cpp
	bindings/python/PythonObject.cpp
)

add_library(joint-python SHARED ${JOINT_PYTHON_BINDING_SRC})
target_link_libraries(joint-python joint-core ${PYTHON_LIBRARIES})

################################################################################

add_executable(joint-test ${CMAKE_CURRENT_SOURCE_DIR}/test/test.cpp)
target_link_libraries(joint-test joint-python)
