#include <binding/PythonObject.hpp>

#include <pyjoint/Object.h>


PythonObject::PythonObject(PyObjectPtr obj)
	: _obj(std::move(obj))
{
	_methods.reset(PyObject_GetAttrString(_obj, "methods"));
	PYTHON_CHECK(_methods, "Could not find methods tuple");
}


PyObjectPtr PythonObject::InvokeMethod(size_t index, joint::ArrayView<const Joint_Variant> params)
{
	auto py_function = PyTuple_GetItem(_methods, index);
	PYTHON_CHECK(py_function, "Could not find method with id " + std::to_string(index));

	PyObjectPtr py_args;
	if (!params.empty())
	{
		py_args.reset(PyTuple_New(params.size()));
		PYTHON_CHECK(py_args, "Could not create args tuple");

		for (size_t i = 0; i < params.size(); ++i)
		{
			auto p = params[i];

			PyObject* py_p = nullptr;
			switch (p.type)
			{
			case JOINT_TYPE_I32:
				py_p = PyLong_FromLong(p.value.i32);
				break;
			case JOINT_TYPE_UTF8:
				py_p = PyUnicode_FromString(p.value.utf8);
				break;
			case JOINT_TYPE_OBJ:
				py_p = PyObject_CallObject((PyObject*)&pyjoint_Object_type, NULL);
				PYJOINT_CHECK(py_p, "Could not create joint.Object");
				PYJOINT_CHECK(Joint_IncRefObject(p.value.obj) == JOINT_ERROR_NONE, "Joint_IncRefObject failed!");
				reinterpret_cast<pyjoint_Object*>(py_p)->handle = p.value.obj;
				break;
			default:
				throw std::runtime_error("Unknown parameter type");
			}

			if (PyTuple_SetItem(py_args, i, py_p)) // TODO: Use PyTuple_SET_ITEM
				PYTHON_ERROR("Could not set tuple item");
		}
	}

	PyObjectPtr py_result(PyObject_CallObject(py_function, py_args));
	PYTHON_CHECK(py_result, "Could not invoke method");
	return py_result;
}
