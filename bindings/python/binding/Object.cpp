#include <binding/Object.hpp>

#include <pyjoint/Object.hpp>


namespace joint_python {
namespace binding
{

	Object::Object(PyObjectHolder obj)
		: _obj(std::move(obj))
	{
		_methods.Reset(PY_OBJ_CHECK(PyObject_GetAttrString(_obj, "methods")));
	}


	PyObjectHolder Object::InvokeMethod(size_t index, joint::ArrayView<const Joint_Variant> params)
	{
		PyObject* py_function = PY_OBJ_CHECK_MSG((PyTuple_GetItem(_methods, index)), "Could not find method with id " + std::to_string(index));

		PyObjectHolder py_args;
		if (!params.empty())
		{
			py_args.Reset(PY_OBJ_CHECK(PyTuple_New(params.size())));

			for (size_t i = 0; i < params.size(); ++i)
			{
				auto p = params[i];

				PyObjectHolder py_p;
				switch (p.type)
				{
				case JOINT_TYPE_BOOL: py_p.Reset(PyBool_FromLong(p.value.b)); break;

				case JOINT_TYPE_U8: py_p.Reset(PyLong_FromLong(p.value.u8)); break;
				case JOINT_TYPE_I8: py_p.Reset(PyLong_FromLong(p.value.i8)); break;
				case JOINT_TYPE_U16: py_p.Reset(PyLong_FromLong(p.value.u16)); break;
				case JOINT_TYPE_I16: py_p.Reset(PyLong_FromLong(p.value.i16)); break;
				case JOINT_TYPE_U32: py_p.Reset(PyLong_FromLong(p.value.u32)); break;
				case JOINT_TYPE_I32: py_p.Reset(PyLong_FromLong(p.value.i32)); break;
				case JOINT_TYPE_U64: py_p.Reset(PyLong_FromLongLong(p.value.u64)); break;
				case JOINT_TYPE_I64: py_p.Reset(PyLong_FromLongLong(p.value.i64)); break;

				case JOINT_TYPE_F32: py_p.Reset(PyFloat_FromDouble(p.value.f32)); break;
				case JOINT_TYPE_F64: py_p.Reset(PyFloat_FromDouble(p.value.f64)); break;

				case JOINT_TYPE_UTF8: py_p.Reset(PyUnicode_FromString(p.value.utf8)); break;

				case JOINT_TYPE_OBJ:
					py_p.Reset(PY_OBJ_CHECK(PyObject_CallObject((PyObject*)&pyjoint::Object_type, NULL)));
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

		PyObjectHolder py_result(PY_OBJ_CHECK(PyObject_CallObject(py_function, py_args)));
		return py_result;
	}

}}
