#include "PythonModule.hpp"

#include <joint/Joint.h>
#include <joint/SomeInterfaceWrapper.hpp>
#include <joint/utils/ArrayView.hpp>
#include <joint/utils/MakeUnique.hpp>

#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>


PythonModule::PythonModule(const std::string& moduleName)
	: _moduleName(moduleName)
{
	wchar_t* argv[1];
	PySys_SetArgv(0, argv);

	PyObjectPtr py_module_name(PyUnicode_FromString(moduleName.c_str()));
	_pyModule.reset(PyImport_Import(py_module_name));

	PYTHON_CHECK(_pyModule, "Could not import python module " + moduleName);

	Joint_Log(JOINT_LOGLEVEL_DEBUG, "PythonBinding", "Loaded python module %s", _moduleName.c_str());
}


PythonModule::~PythonModule()
{
	_pyModule.reset();
	Joint_Log(JOINT_LOGLEVEL_DEBUG, "PythonBinding", "Unloaded python module %s", _moduleName.c_str());
}


//class PythonVariant : public IVariant
//{
//private:
	//PyObjectPtr		_obj;

//public:
	//PythonVariant(PyObjectPtr obj)
		//: _obj(std::move(obj))
	//{ }

	//std::string AsString() const
	//{
		//PyObjectPtr py_bytes(PyUnicode_AsUTF8String(_obj));
		//if (!py_bytes)
			//PYTHON_ERROR("PyUnicode_AsUTF8String failed!");
		//const char* str_data = PyBytes_AsString(py_bytes);
		//if (!str_data)
			//PYTHON_ERROR("PyBytes_AsString failed!");
		//return std::string(str_data);
	//}

	//int32_t AsI32() const
	//{
		//int overflow = 0;
		//long result = PyLong_AsLongAndOverflow(_obj, &overflow);
		//if (overflow != 0)
			//PYTHON_ERROR("Overflow in PyLong_AsLongAndOverflow");
		//return result;
	//}

	//virtual IObjectWrapperPtr AsObject() const;
//};


//class PythonObjectWrapper : public IObjectWrapper
//{
//private:
	//PyObjectPtr		_obj;

//public:
	//PythonObjectWrapper(PyObjectPtr obj)
		//: _obj(std::move(obj))
	//{ }

	//virtual IVariantPtr InvokeMethod(int index, ArrayView<const ParamVariant> params)
	//{
		//std::string method_name = "methodCall";

		//PyObjectPtr py_function(PyObject_GetAttrString(_obj, method_name.c_str()));
		//if (!py_function)
			//PYTHON_ERROR("Could not find method AddRef");

		//PyObjectPtr py_args(PyTuple_New(1 + params.size()));
		//if (!py_args)
			//PYTHON_ERROR("Could not create args tuple");

		//if (PyTuple_SetItem(py_args, 0, PyLong_FromLong(index))) // TODO: Use PyTuple_SET_ITEM
			//PYTHON_ERROR("Could not set tuple item");

		//for (size_t i = 0; i < params.size(); ++i)
		//{
			//auto p = params[i];

			//PyObject* py_p = nullptr;
			//switch (p.GetType())
			//{
			//case ParamVariant::Type::I32: py_p = PyLong_FromLong(p.AsI32()); break;
			//case ParamVariant::Type::String: py_p = PyUnicode_FromString(p.AsString()); break;
			//default: throw std::runtime_error("Unknown parameter type");
			//}

			//if (PyTuple_SetItem(py_args, i + 1, py_p)) // TODO: Use PyTuple_SET_ITEM
				//PYTHON_ERROR("Could not set tuple item");
		//}

		//PyObjectPtr py_result(PyObject_CallObject(py_function, py_args));
		//if (!py_result)
			//PYTHON_ERROR("Could not invoke method");

		//return MakeUnique<PythonVariant>(py_result);
	//}
//};


//IObjectWrapperPtr PythonVariant::AsObject() const
//{ return MakeUnique<PythonObjectWrapper>(_obj); }


PyObjectPtr PythonModule::InvokeFunction(const std::string& functionName)
{
	PyObjectPtr py_function(PyObject_GetAttrString(_pyModule, functionName.c_str()));
	PYTHON_CHECK(py_function, "Could not find function " + functionName + " in python module " + _moduleName);
	PYTHON_CHECK(PyCallable_Check(py_function), functionName + " in python module " + _moduleName + " is not a function");

	PyObjectPtr py_result(PyObject_CallObject(py_function, nullptr));
	PYTHON_CHECK(py_result, functionName + " in python module " + _moduleName + " invokation failed");

	return std::move(py_result);
}
