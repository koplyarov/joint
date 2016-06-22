#include "PythonObject.hpp"


PythonObject::PythonObject(PyObjectPtr obj)
	: _obj(std::move(obj))
{ }


PyObjectPtr PythonObject::InvokeMethod(size_t index, joint::ArrayView<const Joint_Variant> params)
{
	std::string method_name = "methodCall";

	PyObjectPtr py_function(PyObject_GetAttrString(_obj, method_name.c_str()));
	PYTHON_CHECK(py_function, "Could not find method AddRef");

	PyObjectPtr py_args(PyTuple_New(1 + params.size()));
	PYTHON_CHECK(py_args, "Could not create args tuple");

	if (PyTuple_SetItem(py_args, 0, PyLong_FromLong(index))) // TODO: Use PyTuple_SET_ITEM
		PYTHON_ERROR("Could not set tuple item");

	for (size_t i = 0; i < params.size(); ++i)
	{
		auto p = params[i];

		PyObject* py_p = nullptr;
		switch (p.type)
		{
		case JOINT_TYPE_I32:   py_p = PyLong_FromLong(p.value.i32); break;
		case JOINT_TYPE_UTF8:  py_p = PyUnicode_FromString(p.value.utf8); break;
		case JOINT_TYPE_OBJ:
		default: throw std::runtime_error("Unknown parameter type");
		}

		if (PyTuple_SetItem(py_args, i + 1, py_p)) // TODO: Use PyTuple_SET_ITEM
			PYTHON_ERROR("Could not set tuple item");
	}

	PyObjectPtr py_result(PyObject_CallObject(py_function, py_args));
	PYTHON_CHECK(py_result, "Could not invoke method");
	return py_result;
}
