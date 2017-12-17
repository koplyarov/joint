project(joint-python-core)

find_package(PythonLibs REQUIRED)

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    add_definitions(-DMS_NO_COREDLL)
    add_definitions(-DPy_ENABLE_SHARED)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
    add_definitions(-Wall)
    add_definitions(-std=c++11)
endif()

set(JOINT_PYTHON_CORE_SOURCES
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

add_library(joint-python-core SHARED ${JOINT_PYTHON_CORE_SOURCES})
target_link_libraries(joint-python-core joint ${PYTHON_LIBRARIES})
auto_source_group(${JOINT_PYTHON_CORE_SOURCES})

################################################################################

set(JOINT_PYTHON_SOURCES
    JointPython.cpp
)

add_library(joint-python SHARED ${JOINT_PYTHON_SOURCES})
target_link_libraries(joint-python joint-python-core)
auto_source_group(${JOINT_PYTHON_SOURCES})

################################################################################

set(PYJOINT_SOURCES
    PyJoint.cpp
)

add_library(pyjoint SHARED ${PYJOINT_SOURCES})
set_target_properties(pyjoint PROPERTIES PREFIX "")
if (WIN32)
    set_target_properties(pyjoint PROPERTIES SUFFIX ".pyd")
endif()

target_link_libraries(pyjoint joint joint-python-core)
auto_source_group(${PYJOINT_SOURCES})

################################################################################

set(PYJOINT_LOADER_SOURCES
    pyjoint_loader/Globals.cpp
    pyjoint_loader/PyJointLoader.cpp
)

add_library(pyjoint_loader SHARED ${PYJOINT_LOADER_SOURCES})
set_target_properties(pyjoint_loader PROPERTIES PREFIX "")
if (WIN32)
    set_target_properties(pyjoint_loader PROPERTIES SUFFIX ".pyd")
endif()

target_link_libraries(pyjoint_loader joint-python-core joint-loader)
auto_source_group(${PYJOINT_LOADER_SOURCES})

################################################################################

auto_folder(joint-python joint-python-core pyjoint pyjoint_loader)

install(TARGETS
    joint-python-core
    joint-python
    COMPONENT python
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)