project(joint-python-core)

find_package(PythonLibs REQUIRED)

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
	add_definitions(-DMS_NO_COREDLL)
	add_definitions(-DPy_ENABLE_SHARED)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

add_library(joint-python-core SHARED 
	JointPythonCore.cpp
	binding/Binding.cpp
	binding/Module.cpp
	binding/Object.cpp
	pyjoint/Array.cpp
	pyjoint/Common.cpp
	pyjoint/Globals.cpp
	pyjoint/InterfaceDescriptor.cpp
	pyjoint/JointException.cpp
	pyjoint/Module.cpp
	pyjoint/ProxyBase.cpp
	pyjoint/TypeDescriptor.cpp
	utils/PythonUtils.cpp
	common/PythonBindingInfo.cpp
)
target_link_libraries(joint-python-core joint ${PYTHON_LIBRARIES})

################################################################################

add_library(joint-python SHARED JointPython.cpp)
target_link_libraries(joint-python joint-python-core)

################################################################################

add_library(pyjoint SHARED PyJoint.cpp)
set_target_properties(pyjoint PROPERTIES PREFIX "")
if (WIN32)
	set_target_properties(pyjoint PROPERTIES SUFFIX ".pyd")
endif()
target_link_libraries(pyjoint joint joint-python-core)

################################################################################

add_library(pyjoint_loader SHARED
	pyjoint_loader/Globals.cpp
	pyjoint_loader/PyJointLoader.cpp
)
set_target_properties(pyjoint_loader PROPERTIES PREFIX "")
if (WIN32)
	set_target_properties(pyjoint_loader PROPERTIES SUFFIX ".pyd")
endif()
target_link_libraries(pyjoint_loader joint-python-core joint-loader)


install(TARGETS
	joint-python-core
	joint-python
	COMPONENT python
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib)
