#ifndef CORE_JOINT_INTEROP_PYTHONMODULE_HPP
#define CORE_JOINT_INTEROP_PYTHONMODULE_HPP


#include <joint/devkit/Logger.hpp>

#include <string>

#include <utils/PyObjectHolder.hpp>
#include <utils/PythonHeaders.h>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python {
namespace binding
{

	class Module
	{
		JOINT_DEVKIT_LOGGER("Joint.Python.Module")

	private:
		PyObjectHolder	_pyModule;
		std::string		_moduleName;

	public:
		Module(const std::string& moduleName);
		~Module();

		PyObjectHolder InvokeFunction(const std::string& functionName, const PyObjectHolder& jointModule);
	};

}}}

#endif
