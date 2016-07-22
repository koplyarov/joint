project(joint-python-core)

find_package(PythonLibs REQUIRED)

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
	add_definitions(-DMS_NO_COREDLL)
	add_definitions(-DPy_ENABLE_SHARED)
endif()

set(JOINT_PYTHON_CORE_SRC
	JointPythonCore.cpp
	binding/Binding.cpp
	binding/Module.cpp
	binding/Object.cpp
	pyjoint/Common.cpp
	pyjoint/Globals.cpp
	pyjoint/Module.cpp
	pyjoint/Object.cpp
	utils/PythonUtils.cpp
)

add_library(joint-python-core SHARED ${JOINT_PYTHON_CORE_SRC})
target_link_libraries(joint-python-core joint ${PYTHON_LIBRARIES})

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
	set_target_properties(pyjoint PROPERTIES SUFFIX ".pyd")
endif()
target_link_libraries(pyjoint joint joint-python-core)

################################################################################

set(PY_JOINT_LOADER_SRC
	pyjoint_loader/Globals.cpp
	pyjoint_loader/PyJointLoader.cpp
)

add_library(pyjoint_loader SHARED ${PY_JOINT_LOADER_SRC})
set_target_properties(pyjoint_loader PROPERTIES PREFIX "")
if (WIN32)
	set_target_properties(pyjoint_loader PROPERTIES SUFFIX ".pyd")
endif()
target_link_libraries(pyjoint_loader joint-python-core joint-loader)
