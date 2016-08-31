#include <pyjoint/Object.hpp>

#include <joint/devkit/Logger.hpp>
#include <joint/devkit/ScopeExit.hpp>
#include <joint/devkit/StackStorage.hpp>

#include <algorithm>
#include <array>
#include <cstddef>
#include <vector>

#include <utils/PythonUtils.hpp>


namespace joint_python {
namespace pyjoint
{

	using namespace joint::devkit;

	JOINT_DEVKIT_LOGGER("Joint.Python.PyJoint.Object")

	static PyObject* Object_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
	static void Object_del(PyObject* self);
	static PyObject* Object_InvokeMethod(PyObject* self, PyObject* args, PyObject* kwds);

	static PyMethodDef Object_methods[] = {
		{"InvokeMethod", (PyCFunction)Object_InvokeMethod, METH_VARARGS, "InvokeMethod" },
		{NULL}  /* Sentinel */
	};


	static_assert(
		sizeof(unsigned int) == sizeof(Joint_InterfaceChecksum) ||
		sizeof(unsigned long) == sizeof(Joint_InterfaceChecksum),
		"No suitable python member type descriptor for Joint_InterfaceChecksum"
		);
	static const int Joint_InterfaceChecksum_PyType =
		sizeof(unsigned int) == sizeof(Joint_InterfaceChecksum) ? T_UINT :
		(sizeof(unsigned long) == sizeof(Joint_InterfaceChecksum) ? T_ULONG : -1);

	static PyMemberDef Object_members[] =
	{
		{(char*)"checksum", Joint_InterfaceChecksum_PyType, offsetof(Object, checksum), READONLY},
		{NULL}
	};


	PyTypeObject Object_type = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyjoint.Object",          // tp_name
		sizeof(Object),            // tp_basicsize
		0,                         // tp_itemsize
		(destructor)Object_del,    // tp_dealloc
		0,                         // tp_print
		0,                         // tp_getattr
		0,                         // tp_setattr
		0,                         // tp_reserved
		0,                         // tp_repr
		0,                         // tp_as_number
		0,                         // tp_as_sequence
		0,                         // tp_as_mapping
		0,                         // tp_hash
		Object_InvokeMethod,       // tp_call
		0,                         // tp_str
		0,                         // tp_getattro
		0,                         // tp_setattro
		0,                         // tp_as_buffer
		Py_TPFLAGS_DEFAULT,        // tp_flags
		"joint module object",     // tp_doc
		0,                         // tp_traverse
		0,                         // tp_clear
		0,                         // tp_richcompare
		0,                         // tp_weaklistoffset
		0,                         // tp_iter
		0,                         // tp_iternext
		Object_methods,            // tp_methods
		Object_members,            // tp_members
		0,                         // tp_getset
		0,                         // tp_base
		0,                         // tp_dict
		0,                         // tp_descr_get
		0,                         // tp_descr_set
		0,                         // tp_dictoffset
		0,                         // tp_init
		0,                         // tp_alloc
		Object_new                 // tp_new
	};


	static PyObject* Object_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
	{
		PYJOINT_CPP_WRAP_BEGIN

		Object* self = PY_OBJ_CHECK((Object*)type->tp_alloc(type, 0));

		self->handle = JOINT_NULL_HANDLE;
		self->checksum = 0;

		PYJOINT_CPP_WRAP_END((PyObject*)self, NULL)
	}


	static void Object_del(PyObject* self)
	{
		PYJOINT_CPP_WRAP_BEGIN

		auto m = reinterpret_cast<Object*>(self);
		if (m && m->handle)
		{
			Joint_DecRefObject(m->handle);
			m->handle = JOINT_NULL_HANDLE;
		}

		Py_TYPE(self)->tp_free(self);

		PYJOINT_CPP_WRAP_END_VOID()
	}


	static PyObjectHolder ToPyRetValue(const Joint_Type& type, const Joint_Value& value)
	{
		switch (type.id)
		{
		case JOINT_TYPE_BOOL: return PyObjectHolder(PyBool_FromLong(value.b)); break;
		case JOINT_TYPE_I8: return PyObjectHolder(PyLong_FromLong(value.i8)); break;
		case JOINT_TYPE_U8: return PyObjectHolder(PyLong_FromLong(value.u8)); break;
		case JOINT_TYPE_I16: return PyObjectHolder(PyLong_FromLong(value.i16)); break;
		case JOINT_TYPE_U16: return PyObjectHolder(PyLong_FromLong(value.u16)); break;
		case JOINT_TYPE_I32: return PyObjectHolder(PyLong_FromLong(value.i32)); break;
		case JOINT_TYPE_U32: return PyObjectHolder(PyLong_FromLong(value.u32)); break;
		case JOINT_TYPE_I64: return PyObjectHolder(PyLong_FromLong(value.i64)); break;
		case JOINT_TYPE_U64: return PyObjectHolder(PyLong_FromLong(value.u64)); break;
		case JOINT_TYPE_F32: return PyObjectHolder(PyFloat_FromDouble(value.f32)); break;
		case JOINT_TYPE_F64: return PyObjectHolder(PyFloat_FromDouble(value.f64)); break;
		case JOINT_TYPE_UTF8: return PyObjectHolder(PyUnicode_FromString(value.utf8)); break;
		case JOINT_TYPE_ENUM: return PyObjectHolder(PyLong_FromLong(value.e)); break;
		case JOINT_TYPE_STRUCT:
			{
				const auto& sd = *type.payload.structDescriptor;
				PyObjectHolder res(PY_OBJ_CHECK(PyTuple_New(sd.membersCount)));
				for (int32_t i = 0; i < sd.membersCount; ++i)
					PYTHON_CHECK(PyTuple_SetItem(res, i, ToPyRetValue(sd.memberTypes[i], value.members[i]).Release()) == 0, "PyTuple_SetItem failed!");
				return res;
			}
			break;
		case JOINT_TYPE_OBJ:
			if (value.obj != JOINT_NULL_HANDLE)
			{
				PyObjectHolder res(PY_OBJ_CHECK(PyObject_CallObject((PyObject*)&Object_type, NULL)));
				reinterpret_cast<Object*>(res.Get())->handle = value.obj;
				reinterpret_cast<Object*>(res.Get())->checksum = type.payload.interfaceChecksum;
				return res;
			}
			else
			{
				Py_INCREF(Py_None);
				return PyObjectHolder(Py_None);
			}
			break;
		default:
			NATIVE_THROW("Unknown return type: " + std::to_string(type.id));
		}
	}


	class JointFrameData
	{
	private:
		StackStorage<Joint_StructDescriptor, 64>  _structDescriptors;
		StackStorage<Joint_Type, 256>             _memberTypes;
		StackStorage<Joint_Value, 64>             _members;
		StackStorage<PyBytesHolder, 64>           _strParams;

	public:
		Joint_Type MakeJointType(PyObject* pyRetType)
		{
			if (!PyTuple_Check(pyRetType))
			{
				std::string py_type = "<unknown type>";
				PyObjectToStringNoExcept(pyRetType, py_type);
				JOINT_THROW(StringBuilder() % "Expected tuple type, got " % py_type);
			}

			Joint_Type res;
			res.id = FromPyLong<Joint_TypeId>(PY_OBJ_CHECK(PyTuple_GetItem(pyRetType, 0)));
			switch (res.id)
			{
			case JOINT_TYPE_OBJ:
				res.payload.interfaceChecksum = FromPyLong<Joint_InterfaceChecksum>(PY_OBJ_CHECK(PyTuple_GetItem(pyRetType, 1)));
				break;
			case JOINT_TYPE_STRUCT:
				{
					PyObject* py_members = PY_OBJ_CHECK(PyTuple_GetItem(pyRetType, 1));
					auto& sd = *_structDescriptors.Make(1);

					auto tuple_size = PyTuple_Size(py_members);
					sd.membersCount = tuple_size;

					auto mt = _memberTypes.Make(tuple_size);
					sd.memberTypes = mt;

					for (int32_t i = 0; i < tuple_size; ++i)
						mt[i] = MakeJointType(PY_OBJ_CHECK(PyTuple_GetItem(py_members, i)));

					res.payload.structDescriptor = &sd;
				}
				break;
			default:
				break;
			}
			return res;
		}

		Joint_Value MakeJointValue(const Joint_Type& type, PyObject* pyParam)
		{
			Joint_Value res = { };
			switch (type.id)
			{
			case JOINT_TYPE_BOOL: res.b = AsBool(pyParam); break;
			case JOINT_TYPE_U8: res.u8 = FromPyLong<uint8_t>(pyParam); break;
			case JOINT_TYPE_I8: res.i8 = FromPyLong<int8_t>(pyParam); break;
			case JOINT_TYPE_U16: res.u16 = FromPyLong<uint16_t>(pyParam); break;
			case JOINT_TYPE_I16: res.i16 = FromPyLong<int16_t>(pyParam); break;
			case JOINT_TYPE_U32: res.u32 = FromPyLong<uint32_t>(pyParam); break;
			case JOINT_TYPE_I32: res.i32 = FromPyLong<int32_t>(pyParam); break;
			case JOINT_TYPE_U64: res.u64 = FromPyLong<uint64_t>(pyParam); break;
			case JOINT_TYPE_I64: res.i64 = FromPyLong<int64_t>(pyParam); break;
			case JOINT_TYPE_F32: res.f32 = FromPyFloat<float>(pyParam); break;
			case JOINT_TYPE_F64: res.f64 = FromPyFloat<double>(pyParam); break;
			case JOINT_TYPE_ENUM: res.e = FromPyLong<int32_t>(pyParam); break;
			case JOINT_TYPE_UTF8:
				res.utf8 = _strParams.MakeSingle(Utf8FromPyUnicode(pyParam)).GetContent();
				break;
			case JOINT_TYPE_STRUCT:
				{
					if (!PyTuple_Check(pyParam))
					{
						std::string py_type = "<unknown type>";
						PyObjectToStringNoExcept(pyParam, py_type);
						JOINT_THROW(StringBuilder() % "Expected tuple type, got " % py_type);
					}

					const auto& sd = *type.payload.structDescriptor;
					res.members = _members.Make(sd.membersCount);
					for (auto i = 0; i < sd.membersCount; ++i)
					{
						PyObject* py_member = PY_OBJ_CHECK(PyTuple_GetItem(pyParam, i));
						res.members[i] = MakeJointValue(sd.memberTypes[i], py_member);
					}
				}
				break;
			case JOINT_TYPE_OBJ:
				res.obj = (pyParam != Py_None) ? CastPyObject<Object>(pyParam, &Object_type)->handle : JOINT_NULL_HANDLE;
				break;
			default:
				JOINT_THROW(std::runtime_error("Unknown type"));
			}

			return res;
		}
	};


	static PyObject* Object_InvokeMethod(PyObject* self, PyObject* args, PyObject* kwds)
	{
		PYJOINT_CPP_WRAP_BEGIN

		auto o = reinterpret_cast<Object*>(self);
		NATIVE_CHECK(o && o->handle, "Uninitialized object");

		auto tuple_size = PyTuple_Size(args);
		NATIVE_CHECK(tuple_size >= 2, "Could not parse arguments");

		auto method_id = FromPyLong<int>(PY_OBJ_CHECK(PyTuple_GetItem(args, 0)));
		PyObject* py_ret_type = PY_OBJ_CHECK(PyTuple_GetItem(args, 1));

		JointFrameData fd;

		Joint_Type ret_type = fd.MakeJointType(py_ret_type);
		StackStorage<Joint_Parameter, 64> params_storage;

		auto params_count = tuple_size - 2;
		auto params = params_storage.Make(params_count);

		for (auto i = 0; i < params_count; ++i)
		{
			PyObject* py_param_tuple = PY_OBJ_CHECK(PyTuple_GetItem(args, i + 2));
			PyObject* py_param_type = PY_OBJ_CHECK(PyTuple_GetItem(py_param_tuple, 0));
			PyObject* py_param = PyTuple_GetItem(py_param_tuple, 1);

			Joint_Type t = fd.MakeJointType(py_param_type);
			Joint_Value v = fd.MakeJointValue(t, py_param);
			params[i] = Joint_Parameter{v, t};
		}

		Joint_RetValue ret_value;
		Joint_Error ret = Joint_InvokeMethod(o->handle, method_id, params, params_count, ret_type, &ret_value);
		NATIVE_CHECK(ret == JOINT_ERROR_NONE || ret == JOINT_ERROR_EXCEPTION, (std::string("Joint_InvokeMethod failed: ") + Joint_ErrorToString(ret)).c_str());

		PyObjectHolder result;

		if (ret == JOINT_ERROR_NONE)
		{
			auto sg(ScopeExit([&]{
				Joint_Error ret = ret_value.releaseValue(ret_type, ret_value.result.value);
				if (ret != JOINT_ERROR_NONE)
					GetLogger().Error() << "Joint_RetValue::releaseValue failed: " << ret;
			}));

			if (ret_type.id == JOINT_TYPE_VOID)
			{ Py_RETURN_NONE; }

			result = ToPyRetValue(ret_type, ret_value.result.value);
		}
		else if (ret == JOINT_ERROR_EXCEPTION)
		{
			auto sg(ScopeExit([&]{ Joint_ReleaseException(ret_value.result.ex); }));

			Joint_SizeT buf_size = 0;
			StackStorage<char, 256> buf_storage;

			auto ex = ret_value.result.ex;

			Joint_Error ret = Joint_GetExceptionMessageSize(ex, &buf_size);
			if (ret != JOINT_ERROR_NONE)
			{
				Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.Python", "Joint_GetExceptionMessageSize failed: ", Joint_ErrorToString(ret));
				throw std::runtime_error("Could not obtain joint exception message!");
			}

			char* buf = buf_storage.Make(buf_size);
			ret = Joint_GetExceptionMessage(ex, buf, buf_size);
			if (ret != JOINT_ERROR_NONE)
			{
				Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.Python", "Joint_GetExceptionMessage failed: %s", Joint_ErrorToString(ret));
				throw std::runtime_error("Could not obtain joint exception message!");
			}

			PyObjectHolder py_ex(PY_OBJ_CHECK(PyObject_CallObject((PyObject*)&JointException_type, NULL)));
			reinterpret_cast<JointException*>(py_ex.Get())->jointMessage = new std::string(buf);

			Joint_SizeT bt_size = 0;
			ret = Joint_GetExceptionBacktraceSize(ex, &bt_size);
			if (ret != JOINT_ERROR_NONE)
			{
				Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.Python", "Joint_GetExceptionBacktraceSize failed: %s", Joint_ErrorToString(ret));
				PyErr_SetObject((PyObject*)&JointException_type, py_ex);
				return NULL;
			}

			std::vector<StackFrameData> bt;
			bt.reserve(bt_size);
			std::vector<PyObjectHolder> py_frames;

			for (Joint_SizeT i = 0; i < bt_size; ++i)
			{
				Joint_StackFrame sf;
				ret = Joint_GetExceptionBacktraceEntry(ex, i, &sf);
				if (ret != JOINT_ERROR_NONE)
				{
					Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.Python", "Joint_GetExceptionBacktraceEntry failed: %s", Joint_ErrorToString(ret));
					continue;
				}

				bt.emplace_back(StackFrameData(sf.module, sf.filename, sf.line, sf.code, sf.function));
			}

			reinterpret_cast<JointException*>(py_ex.Get())->backtrace = new std::vector<StackFrameData>(bt);
			PyErr_SetObject((PyObject*)&JointException_type, py_ex);

			return NULL;
		}
		else
			NATIVE_THROW((std::string("Joint_InvokeMethod failed: ") + Joint_ErrorToString(ret)).c_str());

		PYJOINT_CPP_WRAP_END(result.Release(), NULL)
	}

}}
