#ifndef CORE_JOINT_INTEROP_PYTHONMODULE_HPP
#define CORE_JOINT_INTEROP_PYTHONMODULE_HPP


#include <joint/interop/PythonUtils.hpp>

#include <string>

#include <Python.h>


namespace joint
{

	struct ISomeInterface
	{
		virtual ~ISomeInterface() { }

		virtual void AddRef() = 0;
		virtual void Release() = 0;
	};

	class PythonModule
	{
		class PyContext
		{
		public:
			PyContext() { Py_Initialize(); }
			~PyContext() { Py_Finalize(); }

			PyContext(const PyContext&) = delete;
			PyContext& operator = (const PyContext&) = delete;
		};

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
