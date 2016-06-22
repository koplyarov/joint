#ifndef BINDINGS_PYTHON_JOINTPYTHONCORE_H
#define BINDINGS_PYTHON_JOINTPYTHONCORE_H


#include <joint/Joint.h>

#include <Python.h>


extern "C"
{
	Joint_Error JointPythonCore_RegisterBinding();
	Joint_Error JointPythonCore_UnregisterBinding();
}

PyMODINIT_FUNC JointPythonCore_InitModule_py3(void); // TODO: try to support both python 2 and 3

#endif
