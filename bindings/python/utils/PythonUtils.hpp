#ifndef JOINT_INTEROP_PYTHONUTILS_HPP
#define JOINT_INTEROP_PYTHONUTILS_HPP


#include <iostream>
#include <sstream>
#include <stdexcept>

#include <Python.h>
#include <utils/PyObjectHolder.hpp>
#include <utils/ScopeExit.hpp>


#define PYTHON_ERROR(...) do { throw std::runtime_error(__VA_ARGS__ + std::string("\n") + ::joint_python::GetPythonErrorMessage()); } while (false)
#define PYTHON_CHECK(Expr_, ...) do { if (!(Expr_)) PYTHON_ERROR(__VA_ARGS__); } while (false)

#define PYJOINT_THROW(Message_) do { throw std::runtime_error(Message_); } while (false)
#define PYJOINT_CHECK(Expr_, Message_) do { if (!(Expr_)) { PYJOINT_THROW(Message_ + std::string("\n") + ::joint_python::GetPythonErrorMessage()); } } while (false)


namespace joint_python
{

	std::string GetPythonErrorMessage();

	template < typename T_ >
	T_ FromPyLong(PyObject* pyLong)
	{
		int overflow = 0;
		long long_result = PyLong_AsLongAndOverflow(pyLong, &overflow);
		PYJOINT_CHECK(!overflow, "long value overflow");
		T_ result = static_cast<T_>(long_result);
		PYJOINT_CHECK(static_cast<long>(result) == long_result, "T_ value overflow");
		return result;
	}


	inline const char* Utf8FromPyUnicode(PyObject* pyStr)
	{
		PyObject* py_bytes(PyUnicode_AsUTF8String(pyStr));
		PYJOINT_CHECK(py_bytes, "PyUnicode_AsUTF8String failed!");
		auto sg = ScopeExit([&]{ Py_DECREF(py_bytes); });
		const char* str_data = PyBytes_AsString(py_bytes);
		PYJOINT_CHECK(str_data, "PyBytes_AsString failed!");
		return str_data;
	}


	template < typename T_ >
	T_* CastPyObject(PyObject* pyObj, PyTypeObject* pyType)
	{
		PYJOINT_CHECK(pyObj && pyType, "Invalid arguments to CastPyObject");
		PYJOINT_CHECK(PyObject_TypeCheck(pyObj, pyType), std::string("Could not cast object to ") + pyType->tp_name);

		return reinterpret_cast<T_*>(pyObj);
	}


	inline std::string GetPythonErrorMessage()
	{
		PyObject *type = NULL, *value = NULL, *traceback = NULL;
		PyErr_Fetch(&type, &value, &traceback);
		auto sg = ScopeExit([&]{ PyErr_Restore(type, value, traceback); });

		std::stringstream result;
		if (type && value && traceback)
		{
			PyObjectHolder py_module_name(PyUnicode_FromString("traceback"));
			PyObjectHolder py_module(PyImport_Import(py_module_name));

			PyObjectHolder py_function(PyObject_GetAttrString(py_module, "format_exception"));
			PYTHON_CHECK(py_function, "Could not find function format_exception in python module traceback");

			PyObjectHolder py_result(PyObject_CallObject(py_function, Py_BuildValue("(O,O,O)", type, value, traceback)));
			PYTHON_CHECK(py_result, "format_exception in python module traceback invokation failed");

			PyObjectHolder seq(PySequence_Fast((PyObject*)py_result, "A sequence expected!"));
			int len = PySequence_Size(seq);
			for (int i = 0; i < len; ++i)
				result << Utf8FromPyUnicode(PySequence_Fast_GET_ITEM(seq.get(), i));
		}
		else
		{
			if (type)
			{
				PyObjectHolder type_str(PyObject_Str(type));
				const char* type_str_data = Utf8FromPyUnicode(type_str.get());
				if (type_str_data)
					result << type_str_data;
			}

			if (value)
			{
				PyObjectHolder value_str(PyObject_Str(value));
				const char* value_str_data = Utf8FromPyUnicode(value_str.get());
				if (value_str_data)
					result << (type ? "\n" : "") << value_str_data;
			}

			if (!type && !value)
				result << "<could not obtain python error info>";
		}

		return result.str();
	}

}

#endif
