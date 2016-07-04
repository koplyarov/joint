#ifndef BINDINGS_PYTHON_JOINTPYTHONCORE_H
#define BINDINGS_PYTHON_JOINTPYTHONCORE_H


#include <joint/Joint.h>

#include <utils/PythonHeaders.h>


#ifdef _MSC_VER
#	ifdef joint_python_core_EXPORTS
#		define JOINT_PYTHON_CORE_API __declspec(dllexport)
#	else
#		define JOINT_PYTHON_CORE_API __declspec(dllimport)
#	endif
#else
#	define JOINT_PYTHON_CORE_API
#endif


extern "C"
{
	JOINT_PYTHON_CORE_API Joint_Error JointPythonCore_RegisterBinding();
	JOINT_PYTHON_CORE_API Joint_Error JointPythonCore_UnregisterBinding();
}

PyMODINIT_FUNC JointPythonCore_InitModule_py3(void);
PyMODINIT_FUNC JointPythonCore_InitModule_py2(void);

#endif
