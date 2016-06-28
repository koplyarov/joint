#include <utils/PythonUtils.hpp>

#include <joint/Joint.h>


namespace joint_python
{

#define PYTHON_THROW_IN_ERROR_GETTER(Message_) do { throw std::runtime_error(Message_); } while (false)
#define PYTHON_CHECK_IN_ERROR_GETTER(Expr_, Message_) do { if (!(Expr_)) PYTHON_THROW_IN_ERROR_GETTER(Message_); } while (false)


	static std::string GetPythonErrorMessageFallback(PyObject* type, PyObject* value, PyObject* traceback)
	{
		std::stringstream result;

		if (type)
		{
			PyObjectHolder type_str(PyObject_Str(type));
			if (type_str)
				result << Utf8FromPyUnicode(type_str.Get()).GetContent();
		}

		if (value)
		{
			PyObjectHolder value_str(PyObject_Str(value));
			if (value_str)
				result << (type ? "\n" : "") << Utf8FromPyUnicode(value_str.Get()).GetContent();
		}

		if (!type && !value)
			result << "<could not obtain python error info>";

		return result.str();
	}

	std::string GetPythonErrorMessage()
	{
		PyObject *type = NULL, *value = NULL, *traceback = NULL;
		PyErr_Fetch(&type, &value, &traceback);
		auto sg = ScopeExit([&]{ PyErr_Restore(type, value, traceback); });

		if (type && value && traceback)
		{
			PyObjectHolder py_module_name(PyUnicode_FromString("traceback"));
			PyObjectHolder py_module(PyImport_Import(py_module_name));

			PyObjectHolder py_function(PyObject_GetAttrString(py_module, "format_exception"));
			if (!py_function)
			{
				Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.Python", "Could not find function format_exception in python module traceback");
				return GetPythonErrorMessageFallback(type, value, traceback);
			}

			PyObjectHolder py_result(PyObject_CallObject(py_function, Py_BuildValue("(O,O,O)", type, value, traceback)));
			if (!py_result)
			{
				Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.Python", "format_exception in python module traceback invokation failed");
				return GetPythonErrorMessageFallback(type, value, traceback);
			}

			std::stringstream result;
			PyObjectHolder seq(PySequence_Fast((PyObject*)py_result, "A sequence expected!"));
			int len = PySequence_Size(seq);
			for (int i = 0; i < len; ++i)
				result << Utf8FromPyUnicode(PySequence_Fast_GET_ITEM(seq.Get(), i)).GetContent();

			return result.str();
		}
		else
			return GetPythonErrorMessageFallback(type, value, traceback);
	}

}
