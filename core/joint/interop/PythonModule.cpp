#include <joint/interop/PythonModule.hpp>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>


namespace joint
{

	PythonModule::PythonModule(const std::string& moduleName)
		: _moduleName(moduleName)
	{
		wchar_t* argv[1];
		PySys_SetArgv(0, argv);

		PyObjectPtr py_module_name(PyUnicode_FromString(moduleName.c_str()));
		_pyModule.reset(PyImport_Import(py_module_name));

		if (!_pyModule)
			PYTHON_ERROR("Could not import python module " + moduleName);
	}


	PythonModule::~PythonModule()
	{
		_pyModule.reset();
	}


	struct IObjectWrapper
	{
		virtual ~IObjectWrapper() { }

		virtual void InvokeMethod(int index) = 0;
	};
	using IObjectWrapperPtr = std::unique_ptr<IObjectWrapper>;


	class MethodDescription
	{
	private:
		std::string		_name;

	public:
		explicit MethodDescription(std::string name)
			: _name(std::move(name))
		{ }

		std::string GetName() const { return _name; }
	};


	class InterfaceDescription
	{
	public:
		using Methods = std::vector<MethodDescription>;

	private:
		std::string		_name;
		Methods			_methods;

	public:
		explicit InterfaceDescription(std::string name, Methods methods)
			: _name(std::move(name)), _methods(std::move(methods))
		{ }

		std::string GetName() const { return _name; }
		const Methods& GetMethods() const { return _methods; }
	};


	class PythonObjectWrapper : public IObjectWrapper
	{
	private:
		const InterfaceDescription*	_interfaceDesc;
		PyObjectPtr					_obj;

	public:
		PythonObjectWrapper(const InterfaceDescription& interfaceDesc, PyObjectPtr obj)
			: _interfaceDesc(&interfaceDesc), _obj(std::move(obj))
		{ }

		virtual void InvokeMethod(int index)
		{
			std::string method_name = _interfaceDesc->GetMethods().at(index).GetName();

			PyObjectPtr py_function(PyObject_GetAttrString(_obj, method_name.c_str()));
			if (!py_function)
				PYTHON_ERROR("Could not find method AddRef");

			PyObjectPtr py_args(PyTuple_New(0));
			if (!py_args)
				PYTHON_ERROR("Could not create args tuple");

			//Py_INCREF(_obj);
			//if (PyTuple_SetItem(py_args, 0, _obj))
				//PYTHON_ERROR("Could set tuple item");

			PyObjectPtr py_result(PyObject_CallObject(py_function, py_args));
			if (!py_result)
				PYTHON_ERROR("Could not invoke method");
		}
	};

	class SomeInterfaceWrapper : public ISomeInterface
	{
	private:
		IObjectWrapperPtr		_obj;

	public:
		SomeInterfaceWrapper(IObjectWrapperPtr obj)
			: _obj(std::move(obj))
		{ }

		virtual void AddRef()
		{ _obj->InvokeMethod(0); }

		virtual void Release()
		{ _obj->InvokeMethod(1); }

		static const InterfaceDescription& GetInterfaceDescription()
		{
			static InterfaceDescription desc("ISomeInterface", { MethodDescription("AddRef"), MethodDescription("Release") });
			return desc;
		}
	};


	ISomeInterface* PythonModule::InvokeFunction(const std::string& functionName)
	{
		PyObjectPtr py_function(PyObject_GetAttrString(_pyModule, functionName.c_str()));
		if (!py_function)
			PYTHON_ERROR("Could not find function " + functionName + " in python module " + _moduleName);

		if (!PyCallable_Check(py_function))
			PYTHON_ERROR(functionName + " in python module " + _moduleName + " is not a function");

		PyObjectPtr py_result(PyObject_CallObject(py_function, nullptr));
		if (!py_result)
			PYTHON_ERROR(functionName + " in python module " + _moduleName + " invokation failed");

		return new SomeInterfaceWrapper(IObjectWrapperPtr(new PythonObjectWrapper(SomeInterfaceWrapper::GetInterfaceDescription(), std::move(py_result))));
	}

}
