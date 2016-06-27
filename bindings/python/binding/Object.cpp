#include <binding/Object.hpp>

#include <pyjoint/Object.h>


namespace joint_python {
namespace binding
{

	Object::Object(PyObjectHolder obj)
		: _obj(std::move(obj))
	{
		_methods.Reset(PyObject_GetAttrString(_obj, "methods"));
		PYTHON_CHECK(_methods, "Could not find methods tuple");
	}


	PyObjectHolder Object::InvokeMethod(size_t index, joint::ArrayView<const Joint_Variant> params)
	{
		PyObject* py_function = PyTuple_GetItem(_methods, index);
		PYTHON_CHECK(py_function, "Could not find method with id " + std::to_string(index));

		PyObjectHolder py_args;
		if (!params.empty())
		{
			py_args.Reset(PyTuple_New(params.size()));
			PYTHON_CHECK(py_args, "Could not create args tuple");

			for (size_t i = 0; i < params.size(); ++i)
			{
				auto p = params[i];

				PyObjectHolder py_p;
				switch (p.type)
				{
				case JOINT_TYPE_I32:
					py_p.Reset(PyLong_FromLong(p.value.i32));
					break;
				case JOINT_TYPE_UTF8:
					py_p.Reset(PyUnicode_FromString(p.value.utf8));
					break;
				case JOINT_TYPE_OBJ:
					py_p.Reset(PyObject_CallObject((PyObject*)&pyjoint::Object_type, NULL));
					PYTHON_CHECK(py_p, "Could not create joint.Object");
					PYTHON_CHECK(Joint_IncRefObject(p.value.obj) == JOINT_ERROR_NONE, "Joint_IncRefObject failed!");
					reinterpret_cast<pyjoint::Object*>(py_p.Get())->handle = p.value.obj;
					break;
				default:
					throw std::runtime_error("Unknown parameter type");
				}

				if (PyTuple_SetItem(py_args, i, py_p.Release())) // TODO: Use PyTuple_SET_ITEM?
					PYTHON_THROW("Could not set tuple item");
			}
		}

		PyObjectHolder py_result(PyObject_CallObject(py_function, py_args));
		PYTHON_CHECK(py_result, "Could not invoke method");
		return py_result;
	}

}}
