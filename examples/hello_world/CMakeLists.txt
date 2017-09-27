set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples/hello-world")
set(JOINT_MODULE_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

find_package(Joint)

################################################################################

include_directories(${JOINT_CPP_INCLUDE_DIRECTORIES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

joint_generate_adapters(c++ runner/adapters.hpp IGreeter.idl)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(example-hello-world runner/adapters.hpp main.cpp)
target_link_libraries(example-hello-world ${JOINT_LIBRARIES})

################################################################################

joint_generate_adapters(python component/adapters.py IGreeter.idl)

add_custom_target(example-hello-world-adapters-py ALL DEPENDS component/adapters.py)

joint_make_module_package(example-hello-world-component-py
	PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}
	MANIFEST component.jm
	DEPENDS example-hello-world-adapters-py
)

joint_add_module_content(example-hello-world-component-py
	DIRECTORY component
	FILES component/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/component/adapters.py
)
