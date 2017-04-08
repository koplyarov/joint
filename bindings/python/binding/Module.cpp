#include "Module.hpp"

#include <joint/Joint.h>
#include <joint/devkit/ArrayView.hpp>
#include <joint/devkit/MakeUnique.hpp>

#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>


namespace joint {
namespace python {
namespace binding
{

	Module::Module(const std::string& location, const std::string& moduleName)
		: _moduleName(moduleName)
	{
#if PY_VERSION_HEX >= 0x03000000
		wchar_t* argv[1];
		PySys_SetArgv(0, argv);

		PyObjectHolder py_importlib_name(PY_OBJ_CHECK(PyUnicode_FromString("importlib")));
		PyObjectHolder py_importlib(PY_OBJ_CHECK(PyImport_Import(py_importlib_name)));
		PyObjectHolder py_find_loader(PY_OBJ_CHECK(PyObject_GetAttrString(py_importlib, "find_loader")));
		PyObjectHolder py_find_loader_params(PY_OBJ_CHECK(Py_BuildValue("(s(s))", _moduleName.c_str(), location.c_str())));
		PyObjectHolder py_loader(PY_OBJ_CHECK(PyObject_CallObject(py_find_loader, py_find_loader_params)));
		_pyModule.Reset(PY_OBJ_CHECK(PyObject_CallMethod(py_loader, "load_module", "s", _moduleName.c_str())));
#else
		char* argv[1];
		PySys_SetArgv(0, argv);
		JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED);
		//import imp
		//m_info = imp.find_module('Package', ['/home/koplyarov/work/joint/build/bin/python/Tests'])
		//m = imp.load_module(*tuple(['Package'] + list(m_info)))
#endif

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

		return py_result;
	}

}}}
