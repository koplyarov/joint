#ifndef CORE_JOINT_INTEROP_PYTHONMODULE_HPP
#define CORE_JOINT_INTEROP_PYTHONMODULE_HPP


#include <string>

#include <utils/PyObjectHolder.hpp>
#include <utils/PythonHeaders.h>
#include <utils/PythonUtils.hpp>


namespace joint_python {
namespace binding
{

	class Module
	{
	private:
		PyObjectHolder		_pyModule;
		std::string		_moduleName;

	public:
		Module(const std::string& moduleName);
		~Module();

		PyObjectHolder InvokeFunction(const std::string& functionName);
	};

}}

#endif
