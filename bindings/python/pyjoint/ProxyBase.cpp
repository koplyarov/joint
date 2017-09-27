#include <pyjoint/ProxyBase.hpp>

#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/util/StackStorage.hpp>

#include <binding/Object.hpp>
#include <common/Marshaller.hpp>
#include <pyjoint/InterfaceDescriptor.hpp>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python {
namespace pyjoint
{

	using namespace devkit;

	JOINT_DEVKIT_LOGGER("Joint.Python.PyJoint.ProxyBase")

	static int ProxyBase_init(ProxyBase* self, PyObject* args, PyObject* kwds);
	static void ProxyBase_del(ProxyBase* self);
	static PyObject* ProxyBase_call(ProxyBase* self, PyObject* args, PyObject* kwds);
	static int ProxyBase_traverse(ProxyBase* self, visitproc visit, void* arg);

	static PyMethodDef ProxyBase_methods[] = {
		{NULL}  /* Sentinel */
	};

	PyTypeObject ProxyBase_type = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyjoint.ProxyBase",                      // tp_name
		sizeof(ProxyBase),                        // tp_basicsize
		0,                                        // tp_itemsize
		(destructor)ProxyBase_del,                // tp_dealloc
		0,                                        // tp_print
		0,                                        // tp_getattr
		0,                                        // tp_setattr
		0,                                        // tp_reserved
		0,                                        // tp_repr
		0,                                        // tp_as_number
		0,                                        // tp_as_sequence
		0,                                        // tp_as_mapping
		0,                                        // tp_hash
		(ternaryfunc)ProxyBase_call,              // tp_call
		0,                                        // tp_str
		0,                                        // tp_getattro
		0,                                        // tp_setattro
		0,                                        // tp_as_buffer
		Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC, // tp_flags
		"joint proxy base object",                // tp_doc
		(traverseproc)ProxyBase_traverse,         // tp_traverse
		0,                                        // tp_clear
		0,                                        // tp_richcompare
		0,                                        // tp_weaklistoffset
		0,                                        // tp_iter
		0,                                        // tp_iternext
		ProxyBase_methods,                        // tp_methods
		NULL,                                     // tp_members
		0,                                        // tp_getset
		0,                                        // tp_base
		0,                                        // tp_dict
		0,                                        // tp_descr_get
		0,                                        // tp_descr_set
		0,                                        // tp_dictoffset
		(initproc)ProxyBase_init,                 // tp_init
		PyType_GenericAlloc,                      // tp_alloc
		PyType_GenericNew                         // tp_new
	};


	static int ProxyBase_init(ProxyBase* self, PyObject* args, PyObject* kwds)
	{
		PYJOINT_CPP_WRAP_BEGIN

		PyObject* py_ifc_desc;
		PYTHON_CHECK(PyArg_ParseTuple(args, "O", &py_ifc_desc), "Could not parse arguments");

		CastPyObject<InterfaceDescriptor>(py_ifc_desc, &InterfaceDescriptor_type);

		Py_XINCREF(py_ifc_desc);
		self->ifcDesc = py_ifc_desc;

		PYJOINT_CPP_WRAP_END(0, -1, Py_DECREF(self);)
	}


	static void ProxyBase_del(ProxyBase* self)
	{
		PYJOINT_CPP_WRAP_BEGIN

		JOINT_CORE_DECREF_ACCESSOR(self->obj);
		Py_XDECREF(self->ifcDesc);
		Py_TYPE(self)->tp_free((PyObject*)self);

		PYJOINT_CPP_WRAP_END_VOID()
	}


	static PyObject* ProxyBase_call(ProxyBase* self, PyObject* args, PyObject* kwds)
	{
		PYJOINT_CPP_WRAP_BEGIN

		auto ifc_desc = reinterpret_cast<InterfaceDescriptor*>(self->ifcDesc);
		NATIVE_CHECK(ifc_desc, "Invalid object");

		auto tuple_size = PyTuple_Size(args);
		NATIVE_CHECK(tuple_size >= 1, "Could not parse arguments");

		auto method_id = FromPyLong<int>(PY_OBJ_CHECK(PyTuple_GetItem(args, 0)));
		const auto& method_desc = ifc_desc->GetDescriptor().GetMethod(method_id);

		StackStorage<JointCore_Parameter, 64> params_storage;

		auto params_count = tuple_size - 1;
		auto params = params_storage.Make(params_count);

		ParamsAllocator alloc;

		for (auto i = 0; i < params_count; ++i)
		{
			PyObject* py_param = PyTuple_GetItem(args, i + 1);

			const auto& t = method_desc.GetParamType(i);
			JointCore_Value v = ValueMarshaller::ToJoint(ValueDirection::Parameter, t, py_param, PythonMarshaller(), alloc);
			params[i] = JointCore_Parameter{v};
		}

		JointCore_Type ret_type = method_desc.GetRetType().GetJointType();
		JointCore_RetValue ret_value;
		JOINT_CHECK(!JOINT_CORE_IS_NULL(self->obj), JOINT_CORE_ERROR_INVALID_PARAMETER);
		JointCore_Error ret = self->obj.VTable->InvokeMethod(self->obj.Instance, method_id, params, params_count, &ret_value);
		NATIVE_CHECK(ret == JOINT_CORE_ERROR_NONE || ret == JOINT_CORE_ERROR_EXCEPTION, (std::string("Joint_InvokeMethod failed: ") + JointCore_ErrorToString(ret)).c_str());

		PyObjectHolder result;

		if (ret == JOINT_CORE_ERROR_NONE)
		{
			auto sg(ScopeExit([&]{
				JointCore_Error ret = ret_value.releaseValue(ret_type, ret_value.result.value);
				if (ret != JOINT_CORE_ERROR_NONE)
					GetLogger().Error() << "JointCore_RetValue::releaseValue failed: " << ret;
			}));

			if (ret_type.id == JOINT_CORE_TYPE_VOID)
			{ Py_RETURN_NONE; }

			result = ValueMarshaller::FromJoint<PyObjectHolder>(ValueDirection::Return, method_desc.GetRetType(), ret_value.result.value, PythonMarshaller());
		}
		else if (ret == JOINT_CORE_ERROR_EXCEPTION)
		{
			auto sg(ScopeExit([&]{ Joint_ReleaseException(ret_value.result.ex); }));

			JointCore_SizeT buf_size = 0;
			StackStorage<char, 256> buf_storage;

			auto ex = ret_value.result.ex;

			JointCore_Error ret = Joint_GetExceptionMessageSize(ex, &buf_size);
			if (ret != JOINT_CORE_ERROR_NONE)
			{
				Joint_Log(JOINT_CORE_LOGLEVEL_ERROR, "Joint.Python", "Joint_GetExceptionMessageSize failed: ", JointCore_ErrorToString(ret));
				throw std::runtime_error("Could not obtain joint exception message!");
			}

			char* buf = buf_storage.Make(buf_size);
			ret = Joint_GetExceptionMessage(ex, buf, buf_size);
			if (ret != JOINT_CORE_ERROR_NONE)
			{
				Joint_Log(JOINT_CORE_LOGLEVEL_ERROR, "Joint.Python", "Joint_GetExceptionMessage failed: %s", JointCore_ErrorToString(ret));
				throw std::runtime_error("Could not obtain joint exception message!");
			}

			PyObjectHolder py_ex(PY_OBJ_CHECK(PyObject_CallObject((PyObject*)&JointException_type, NULL)));
			reinterpret_cast<JointException*>(py_ex.Get())->jointMessage = new std::string(buf);

			JointCore_SizeT bt_size = 0;
			ret = Joint_GetExceptionBacktraceSize(ex, &bt_size);
			if (ret != JOINT_CORE_ERROR_NONE)
			{
				Joint_Log(JOINT_CORE_LOGLEVEL_WARNING, "Joint.Python", "Joint_GetExceptionBacktraceSize failed: %s", JointCore_ErrorToString(ret));
				PyErr_SetObject((PyObject*)&JointException_type, py_ex);
				return NULL;
			}

			std::vector<StackFrameData> bt;
			bt.reserve(bt_size);
			std::vector<PyObjectHolder> py_frames;

			for (JointCore_SizeT i = 0; i < bt_size; ++i)
			{
				JointCore_StackFrame sf;
				ret = Joint_GetExceptionBacktraceEntry(ex, i, &sf);
				if (ret != JOINT_CORE_ERROR_NONE)
				{
					Joint_Log(JOINT_CORE_LOGLEVEL_WARNING, "Joint.Python", "Joint_GetExceptionBacktraceEntry failed: %s", JointCore_ErrorToString(ret));
					continue;
				}

				bt.emplace_back(StackFrameData(sf.module, sf.filename, sf.line, sf.code, sf.function));
			}

			reinterpret_cast<JointException*>(py_ex.Get())->backtrace = new std::vector<StackFrameData>(bt);
			PyErr_SetObject((PyObject*)&JointException_type, py_ex);

			return NULL;
		}
		else
			NATIVE_THROW((std::string("Joint_InvokeMethod failed: ") + JointCore_ErrorToString(ret)).c_str());

		PYJOINT_CPP_WRAP_END(result.Release(), NULL)
	}


	static int ProxyBase_traverse(ProxyBase* self, visitproc visit, void* arg)
	{
		if (self->ifcDesc)
			Py_VISIT(self->ifcDesc);

		return 0;
	}

}}}
