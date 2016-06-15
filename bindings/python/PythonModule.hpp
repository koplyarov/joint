#ifndef CORE_JOINT_INTEROP_PYTHONMODULE_HPP
#define CORE_JOINT_INTEROP_PYTHONMODULE_HPP


#include <joint/ISomeInterface.hpp>

#include <string>

#include <Python.h>

#include "PythonUtils.hpp"
#include "PythonContext.hpp"


namespace joint
{

	class PythonModule
	{
	private:
		PyContext		_pyCtx;
		PyObjectPtr		_pyModule;
		std::string		_moduleName;

	public:
		PythonModule(const std::string& moduleName);
		~PythonModule();

		ISomeInterface* InvokeFunction(const std::string& functionName);
	};

}

#endif
