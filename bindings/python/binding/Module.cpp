#include "Module.hpp"

#include <joint/Joint.h>
#include <joint/utils/ArrayView.hpp>
#include <joint/utils/MakeUnique.hpp>

#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>


namespace joint_python {
namespace binding
{

	Module::Module(const std::string& moduleName)
		: _moduleName(moduleName)
	{
#if PY_VERSION_HEX >= 0x03000000
		wchar_t* argv[1];
		PySys_SetArgv(0, argv);
#else
		char* argv[1];
		PySys_SetArgv(0, argv);
#endif

		PyObjectHolder py_module_name(PyUnicode_FromString(moduleName.c_str()));
		_pyModule.Reset(PyImport_Import(py_module_name));

		PYTHON_CHECK(_pyModule, "Could not import python module " + moduleName);

		GetLogger().Debug() << "Loaded python module" << _moduleName;
	}


	Module::~Module()
	{
		_pyModule.Reset();
		GetLogger().Debug() << "Unloaded python module" << _moduleName;
	}


	PyObjectHolder Module::InvokeFunction(const std::string& functionName)
	{
		PyObjectHolder py_function(PyObject_GetAttrString(_pyModule, functionName.c_str()));
		PYTHON_CHECK(py_function, "Could not find function " + functionName + " in python module " + _moduleName);
		PYTHON_CHECK(PyCallable_Check(py_function), functionName + " in python module " + _moduleName + " is not a function");

		PyObjectHolder py_result(PyObject_CallObject(py_function, nullptr));
		PYTHON_CHECK(py_result, functionName + " in python module " + _moduleName + " invokation failed");

		return std::move(py_result);
	}

}}
