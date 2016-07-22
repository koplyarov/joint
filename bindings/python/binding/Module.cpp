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

		PyObjectHolder py_module_name(PY_OBJ_CHECK(PyUnicode_FromString(moduleName.c_str())));
		_pyModule.Reset(PY_OBJ_CHECK_MSG(PyImport_Import(py_module_name), "Could not import python module " + moduleName));

		GetLogger().Debug() << "Loaded python module " << _moduleName;
	}


	Module::~Module()
	{
		_pyModule.Reset();
		GetLogger().Debug() << "Unloaded python module " << _moduleName;
	}


	PyObjectHolder Module::InvokeFunction(const std::string& functionName, const PyObjectHolder& jointModule)
	{
		PyObjectHolder py_function(PY_OBJ_CHECK_MSG(PyObject_GetAttrString(_pyModule, functionName.c_str()), "Could not find function " + functionName + " in python module " + _moduleName));
		PyObjectHolder py_params(PY_OBJ_CHECK(Py_BuildValue("(O)", jointModule.Get())));
		PyObjectHolder py_result(PY_OBJ_CHECK_MSG(PyObject_CallObject(py_function, py_params), functionName + " in python module " + _moduleName + " invokation failed"));

		return std::move(py_result);
	}

}}
