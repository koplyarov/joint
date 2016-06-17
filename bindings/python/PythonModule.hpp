#ifndef CORE_JOINT_INTEROP_PYTHONMODULE_HPP
#define CORE_JOINT_INTEROP_PYTHONMODULE_HPP


#include <string>

#include <Python.h>

#include "PythonUtils.hpp"


class PythonModule
{
private:
	PyObjectPtr		_pyModule;
	std::string		_moduleName;

public:
	PythonModule(const std::string& moduleName);
	~PythonModule();

	PyObjectPtr InvokeFunction(const std::string& functionName);
};


#endif
