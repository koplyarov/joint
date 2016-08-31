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


	static PyObjectHolder ToPyValue(const Joint_Type& type, const Joint_Value& value)
	{
		switch (type.id)
		{
		case JOINT_TYPE_BOOL: return PyObjectHolder(PyBool_FromLong(value.b));
		case JOINT_TYPE_U8: return PyObjectHolder(PyLong_FromLong(value.u8));
		case JOINT_TYPE_I8: return PyObjectHolder(PyLong_FromLong(value.i8));
		case JOINT_TYPE_U16: return PyObjectHolder(PyLong_FromLong(value.u16));
		case JOINT_TYPE_I16: return PyObjectHolder(PyLong_FromLong(value.i16));
		case JOINT_TYPE_U32: return PyObjectHolder(PyLong_FromLong(value.u32));
		case JOINT_TYPE_I32: return PyObjectHolder(PyLong_FromLong(value.i32));
		case JOINT_TYPE_U64: return PyObjectHolder(PyLong_FromLongLong(value.u64));
		case JOINT_TYPE_I64: return PyObjectHolder(PyLong_FromLongLong(value.i64));
		case JOINT_TYPE_F32: return PyObjectHolder(PyFloat_FromDouble(value.f32));
		case JOINT_TYPE_F64: return PyObjectHolder(PyFloat_FromDouble(value.f64));
		case JOINT_TYPE_UTF8: return PyObjectHolder(PyUnicode_FromString(value.utf8));
		case JOINT_TYPE_ENUM: return PyObjectHolder(PyLong_FromLong(value.e));
		case JOINT_TYPE_STRUCT:
			{
				const auto& sd = *type.payload.structDescriptor;
				PyObjectHolder res(PY_OBJ_CHECK(PyTuple_New(sd.membersCount)));
				for (int32_t i = 0; i < sd.membersCount; ++i)
					PYTHON_CHECK(PyTuple_SetItem(res, i, ToPyValue(sd.memberTypes[i], value.members[i]).Release()) == 0, "PyTuple_SetItem failed!");
				return res;
			}
			break;
		case JOINT_TYPE_OBJ:
			if (value.obj != JOINT_NULL_HANDLE)
			{
				PyObjectHolder res(PY_OBJ_CHECK(PyObject_CallObject((PyObject*)&pyjoint::Object_type, NULL)));
				Joint_IncRefObject(value.obj);
				reinterpret_cast<pyjoint::Object*>(res.Get())->handle = value.obj;
				reinterpret_cast<pyjoint::Object*>(res.Get())->checksum = type.payload.interfaceChecksum;
				return res;
			}
			else
			{
				Py_INCREF(Py_None);
				return PyObjectHolder(Py_None);
			}
			break;
		default:
			throw std::runtime_error("Unknown parameter type");
		}
	}

	PyObjectHolder Object::InvokeMethod(size_t index, joint::ArrayView<const Joint_Parameter> params)
	{
		PyObject* py_function = PY_OBJ_CHECK_MSG((PyTuple_GetItem(_methods, index)), "Could not find method with id " + std::to_string(index));

		PyObjectHolder py_args;
		if (!params.empty())
		{
			py_args.Reset(PY_OBJ_CHECK(PyTuple_New(params.size())));

			for (size_t i = 0; i < params.size(); ++i)
			{
				auto p = params[i];

				if (PyTuple_SetItem(py_args, i, ToPyValue(p.type, p.value).Release())) // TODO: Use PyTuple_SET_ITEM?
					PYTHON_THROW("Could not set tuple item");
			}
		}

		return PyObjectHolder(PyObject_CallObject(py_function, py_args));
	}

}}
