#ifndef CORE_JOINT_INTEROP_PYTHONMODULE_HPP
#define CORE_JOINT_INTEROP_PYTHONMODULE_HPP


#include <string>

#include <Python.h>


namespace joint
{

	class PythonModule
	{
	private:

	public:
		PythonModule(const std::string& moduleName);
		~PythonModule();
	};

}

#endif
