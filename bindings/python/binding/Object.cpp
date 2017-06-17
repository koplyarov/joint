#include <binding/Object.hpp>

#include <joint/devkit/CppWrappers.hpp>
#include <joint/devkit/ValueMarshaller.hpp>
#include <joint/devkit/accessors/MakeAccessor.hpp>

#include <algorithm>

#include <common/Marshaller.hpp>
#include <pyjoint/Globals.hpp>
#include <pyjoint/InterfaceDescriptor.hpp>
#include <pyjoint/ProxyBase.hpp>


namespace joint {
namespace python {
namespace binding
{

	using namespace devkit;


	Object::Object(PyObjectHolder obj)
		: _obj(std::move(obj))
	{
		_methods.Reset(PY_OBJ_CHECK(PyObject_GetAttrString(_obj, "methods")));
		_interfaceDesc.Reset(PY_OBJ_CHECK(PyObject_GetAttrString(_obj, "descriptor")));
	}


	JointCore_Error Object::CastObject(JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectAccessor* outAccessor) JOINT_DEVKIT_NOEXCEPT
	{
		JOINT_CPP_WRAP_BEGIN

		PyObjectHolder py_obj(PY_OBJ_CHECK(PyObject_GetAttrString(_obj, "obj")));
		PyObjectHolder py_obj_type(PyObject_Type(py_obj));
		PyObjectHolder base_type = FindBaseById(py_obj_type, interfaceId);
		if (!base_type)
			return JOINT_CORE_ERROR_CAST_FAILED;

		PyObjectHolder py_checksum(PY_OBJ_CHECK(PyObject_GetAttrString(base_type, "interfaceChecksum")));
		if (FromPyLong<JointCore_InterfaceChecksum>(py_checksum) != checksum)
			return JOINT_CORE_ERROR_INVALID_INTERFACE_CHECKSUM;

		PyObjectHolder base_accessor_type(PY_OBJ_CHECK(PyObject_GetAttrString(base_type, "accessor")));

		PyObjectHolder py_params(Py_BuildValue("(O)", (PyObject*)py_obj));
		PyObjectHolder new_accessor(PY_OBJ_CHECK(PyObject_CallObject(base_accessor_type, py_params)));
		*outAccessor = accessors::MakeAccessor<Object>(new_accessor);

		JOINT_CPP_WRAP_END
	}

	JointCore_Error Object::InvokeMethod(JointCore_SizeT methodId, const JointCore_Parameter* paramsPtr, JointCore_SizeT paramsCount, JointCore_Type retType, JointCore_RetValue* outRetValue) JOINT_DEVKIT_NOEXCEPT
	{
		JOINT_CPP_WRAP_BEGIN

		joint::ArrayView<const JointCore_Parameter> params(paramsPtr, paramsCount);

		PyObject* py_function = PY_OBJ_CHECK_MSG((PyTuple_GetItem(_methods, methodId)), "Could not find method with id " + std::to_string(methodId));
		auto ifc_desc = CastPyObject<pyjoint::InterfaceDescriptor>(_interfaceDesc, &pyjoint::InterfaceDescriptor_type);
		NATIVE_CHECK(ifc_desc, "Invalid InterfaceDescriptor object");
		const auto& m_desc = ifc_desc->GetDescriptor().GetMethod(methodId);

		PyObjectHolder py_args;
		if (!params.empty())
		{
			py_args.Reset(PY_OBJ_CHECK(PyTuple_New(params.size())));

			for (size_t i = 0; i < params.size(); ++i)
			{
				auto py_p = ValueMarshaller::FromJoint<PyObjectHolder>(ValueDirection::Parameter, m_desc.GetParamType(i), params[i].value, PythonMarshaller());
				if (PyTuple_SetItem(py_args, i, py_p.Release()))
					PYTHON_THROW("Could not set tuple item");
			}
		}

		PyObjectHolder py_res(PyObject_CallObject(py_function, py_args));

		if (!py_res)
		{
			PyObjectHolder type, value, traceback;
			PyErr_Fetch(&type, &value, &traceback);
			pyjoint::JointException* ex = nullptr;

			std::string msg = "<No message>";

			do
			{
				if (PyErr_GivenExceptionMatches(type, (PyObject*)&pyjoint::JointException_type))
				{
					PyErr_NormalizeException(&type, &value, &traceback);
					if (PyObject_Type(value) == (PyObject*)&pyjoint::JointException_type)
					{
						ex = reinterpret_cast<pyjoint::JointException*>(value.Get());
						if (ex->jointMessage)
							msg = *ex->jointMessage;
						break;
					}
					else
						GetLogger().Warning() << "Exception that matches JointException_type has value of a different type!";
				}
				else if (PyErr_GivenExceptionMatches(type, pyjoint::InvalidInterfaceChecksumException))
					GetLogger().Error() << "Invalid interface checksum!";
				std::string type_str;
				GetPythonErrorMessage(value, msg);
				if (PyObjectToStringNoExcept(type, type_str))
					msg = type_str + " " + msg;
			} while (false);


			std::vector<devkit::StackFrameData> bt;
			GetPythonErrorBacktrace(traceback, bt);

			std::vector<JointCore_StackFrame> c_bt;
			c_bt.reserve(bt.size() + (ex && ex->backtrace ? ex->backtrace->size() : 0));

			auto tr_f = [](const devkit::StackFrameData& sf) {
					return JointCore_StackFrame{sf.GetModule().c_str(), sf.GetFilename().c_str(), sf.GetLine(), sf.GetCode().c_str(), sf.GetFunction().c_str()};
				};

			if (ex && ex->backtrace)
				std::transform(ex->backtrace->begin(), ex->backtrace->end(), std::back_inserter(c_bt), tr_f);
			std::transform(bt.rbegin(), bt.rend(), std::back_inserter(c_bt), tr_f);

			JointCore_ExceptionHandle joint_ex;
			JointCore_Error ret = Joint_MakeException(msg.c_str(), c_bt.data(), c_bt.size(), &joint_ex);
			JOINT_CHECK(ret == JOINT_CORE_ERROR_NONE, std::string("Joint_MakeException failed: ") + JointCore_ErrorToString(ret));
			outRetValue->releaseValue = &Object::ReleaseRetValue;
			outRetValue->result.ex = joint_ex;

			return JOINT_CORE_ERROR_EXCEPTION;
		}

		RetValueAllocator alloc;
		if (retType.id != JOINT_CORE_TYPE_VOID)
			outRetValue->result.value = ValueMarshaller::ToJoint(ValueDirection::Return, m_desc.GetRetType(), py_res, PythonMarshaller(), alloc);
		outRetValue->releaseValue = &Object::ReleaseRetValue;
		return JOINT_CORE_ERROR_NONE;

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Object::ReleaseRetValue(JointCore_Type type, JointCore_Value value)
	{
		JOINT_CPP_WRAP_BEGIN
		switch(type.id)
		{
		case JOINT_CORE_TYPE_UTF8:
			delete[] value.utf8;
			break;
		case JOINT_CORE_TYPE_STRUCT:
			for (int32_t i = 0; i < type.payload.structDescriptor->membersCount; ++i)
				ReleaseRetValue(type.payload.structDescriptor->memberTypes[i], value.members[i]);
			delete[] value.members;
			break;
		default:
			break;
		}
		JOINT_CPP_WRAP_END
	}

	PyObjectHolder Object::FindBaseById(PyObject* type, const char* interfaceId)
	{
		PyObjectHolder bases(PyObject_GetAttrString(type, "__bases__"));
		PyObjectHolder seq(PySequence_Fast((PyObject*)bases, "A sequence expected!"));
		int len = PySequence_Size(seq);
		for (int i = 0; i < len; ++i)
		{
			PyObject* base = PY_OBJ_CHECK(PySequence_Fast_GET_ITEM(seq.Get(), i));

			if (!PyObject_HasAttrString(base, "interfaceId"))
				continue;

#if PY_VERSION_HEX >= 0x03000000
			PyObjectHolder py_base_interface_id(PY_OBJ_CHECK(PyObject_GetAttrString(base, "interfaceId")));
#else
			PyObjectHolder py_base_interface_id_attr(PY_OBJ_CHECK(PyObject_GetAttrString(base, "interfaceId")));
			PyObjectHolder py_base_interface_id(PY_OBJ_CHECK(PyObject_Unicode(py_base_interface_id_attr)));
#endif

			auto base_interface_id = Utf8FromPyUnicode(py_base_interface_id);
			if (strcmp(interfaceId, base_interface_id.GetContent()) == 0)
			{
				Py_INCREF(base);
				return PyObjectHolder(base);
			}

			PyObjectHolder in_base = FindBaseById(base, interfaceId);
			if (in_base)
				return in_base;
		}

		return PyObjectHolder();
	}

}}}
