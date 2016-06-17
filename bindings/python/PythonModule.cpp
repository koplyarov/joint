#include "PythonModule.hpp"

#include <joint/Joint.h>
#include <joint/SomeInterfaceWrapper.hpp>
#include <joint/utils/ArrayView.hpp>
#include <joint/utils/MakeUnique.hpp>

#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>


PythonModule::PythonModule(const std::string& moduleName)
	: _moduleName(moduleName)
{
	wchar_t* argv[1];
	PySys_SetArgv(0, argv);

	PyObjectPtr py_module_name(PyUnicode_FromString(moduleName.c_str()));
	_pyModule.reset(PyImport_Import(py_module_name));

	PYTHON_CHECK(_pyModule, "Could not import python module " + moduleName);

	Joint_Log(JOINT_LOGLEVEL_DEBUG, "PythonBinding", "Loaded python module %s", _moduleName.c_str());
}


PythonModule::~PythonModule()
{
	_pyModule.reset();
	Joint_Log(JOINT_LOGLEVEL_DEBUG, "PythonBinding", "Unloaded python module %s", _moduleName.c_str());
}


PyObjectPtr PythonModule::InvokeFunction(const std::string& functionName)
{
	PyObjectPtr py_function(PyObject_GetAttrString(_pyModule, functionName.c_str()));
	PYTHON_CHECK(py_function, "Could not find function " + functionName + " in python module " + _moduleName);
	PYTHON_CHECK(PyCallable_Check(py_function), functionName + " in python module " + _moduleName + " is not a function");

	PyObjectPtr py_result(PyObject_CallObject(py_function, nullptr));
	PYTHON_CHECK(py_result, functionName + " in python module " + _moduleName + " invokation failed");

	return std::move(py_result);
}
