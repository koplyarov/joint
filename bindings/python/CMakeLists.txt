project(joint-python-core)

find_package(PythonLibs REQUIRED)

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(JOINT_PYTHON_CORE_SRC
	JointPythonCore.cpp
	binding/Binding.cpp
	binding/Module.cpp
	binding/Object.cpp
	pyjoint/Common.cpp
	pyjoint/Globals.cpp
	pyjoint/Module.cpp
	pyjoint/Object.cpp
)

add_library(joint-python-core SHARED ${JOINT_PYTHON_CORE_SRC})
target_link_libraries(joint-python-core joint-core ${PYTHON_LIBRARIES})

################################################################################

set(JOINT_PYTHON_BINDING_SRC
	JointPython.cpp
)

add_library(joint-python SHARED ${JOINT_PYTHON_BINDING_SRC})
target_link_libraries(joint-python joint-python-core)

################################################################################

set(PY_JOINT_SRC
	PyJoint.cpp
)

add_library(pyjoint SHARED ${PY_JOINT_SRC})
set_target_properties(pyjoint PROPERTIES PREFIX "")
if (WIN32)
	set_target_properties(pyjoint PROPERTIES SUFFIX "pyd")
endif()
target_link_libraries(pyjoint joint-python-core)
