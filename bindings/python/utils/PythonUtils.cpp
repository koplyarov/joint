#include <utils/PythonUtils.hpp>

#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>


namespace joint_python
{

	JOINT_DEVKIT_LOGGER("Joint.Python.Utils")

#define PYTHON_THROW_IN_ERROR_GETTER(Message_) do { throw std::runtime_error(Message_); } while (false)
#define PYTHON_CHECK_IN_ERROR_GETTER(Expr_, Message_) do { if (!(Expr_)) PYTHON_THROW_IN_ERROR_GETTER(Message_); } while (false)


	static std::string GetPythonErrorMessageFallback(PyObject* type, PyObject* value, PyObject* traceback)
	{
		std::stringstream result;

		if (type)
		{
#if PY_VERSION_HEX >= 0x03000000
			PyObjectHolder type_str(PyObject_Str(type));
#else
			PyObjectHolder type_str(PyObject_Unicode(type));
#endif
			if (type_str)
			{
				PyObjectHolder py_bytes(PyUnicode_AsUTF8String(type_str));
				if (py_bytes)
				{
					const char* content = PyBytes_AsString(py_bytes);
					if (content)
						result << content;
					else
						result << "<Could not obtain exception type: content == NULL>";
				}
				else
					result << "<Could not obtain exception type: py_bytes == NULL>";
			}
		}

		if (value)
		{
#if PY_VERSION_HEX >= 0x03000000
			PyObjectHolder value_str(PyObject_Str(value));
#else
			PyObjectHolder value_str(PyObject_Unicode(value));
#endif
			if (value_str)
			{
				PyObjectHolder py_bytes(PyUnicode_AsUTF8String(value_str));
				if (py_bytes)
				{
					const char* content = PyBytes_AsString(py_bytes);
					if (content)
						result << (type ? "\n" : "") << content;
					else
						result << "<Could not obtain exception message: content == NULL>";
				}
				else
					result << "<Could not obtain exception message: py_bytes == NULL>";
			}
		}

		if (!type && !value)
			result << "<Could not obtain python error info: type == NULL && value == NULL>";

		return result.str();
	}

	std::string GetPythonErrorMessage()
	{
		PyObject *type = NULL, *value = NULL, *traceback = NULL;
		PyErr_Fetch(&type, &value, &traceback);
		auto sg = ScopeExit([&]{ PyErr_Restore(type, value, traceback); PyErr_Clear(); });

		if (type && value && traceback)
		{
			PyObjectHolder py_module_name(PyUnicode_FromString("traceback"));
			PyObjectHolder py_module(PyImport_Import(py_module_name));

			PyObjectHolder py_function(PyObject_GetAttrString(py_module, "format_exception"));
			if (!py_function)
			{
				GetLogger().Warning() << "Could not find function format_exception in python module traceback";
				return GetPythonErrorMessageFallback(type, value, traceback);
			}

			PyObjectHolder py_result(PyObject_CallObject(py_function, Py_BuildValue("(O,O,O)", type, value, traceback)));
			if (!py_result)
			{
				GetLogger().Warning() << "format_exception in python module traceback invokation failed";
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
