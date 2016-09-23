#include <binding/Object.hpp>

#include <joint/devkit/ValueMarshaller.hpp>
#include <joint/utils/CppWrappers.hpp>

#include <algorithm>

#include <common/Marshaller.hpp>
#include <pyjoint/Globals.hpp>
#include <pyjoint/InterfaceDescriptor.hpp>
#include <pyjoint/ProxyBase.hpp>


namespace joint_python {
namespace binding
{

	using namespace joint::devkit;


	Object::Object(PyObjectHolder obj)
		: _obj(std::move(obj))
	{
		_methods.Reset(PY_OBJ_CHECK(PyObject_GetAttrString(_obj, "methods")));
		_interfaceDesc.Reset(PY_OBJ_CHECK(PyObject_GetAttrString(_obj, "descriptor")));
	}


	Joint_Error Object::InvokeMethod(size_t index, joint::ArrayView<const Joint_Parameter> params, Joint_Type retType, Joint_RetValue* outRetValue)
	{
		PyObject* py_function = PY_OBJ_CHECK_MSG((PyTuple_GetItem(_methods, index)), "Could not find method with id " + std::to_string(index));
		auto ifc_desc = CastPyObject<pyjoint::InterfaceDescriptor>(_interfaceDesc, &pyjoint::InterfaceDescriptor_type);
		NATIVE_CHECK(ifc_desc, "Invalid InterfaceDescriptor object");
		const auto& m_desc = ifc_desc->GetDescriptor().GetMethod(index);

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


			std::vector<joint::devkit::StackFrameData> bt;
			GetPythonErrorBacktrace(traceback, bt);

			std::vector<Joint_StackFrame> c_bt;
			c_bt.reserve(bt.size() + (ex && ex->backtrace ? ex->backtrace->size() : 0));

			auto tr_f = [](const joint::devkit::StackFrameData& sf) {
					return Joint_StackFrame{sf.GetModule().c_str(), sf.GetFilename().c_str(), sf.GetLine(), sf.GetCode().c_str(), sf.GetFunction().c_str()};
				};

			if (ex && ex->backtrace)
				std::transform(ex->backtrace->begin(), ex->backtrace->end(), std::back_inserter(c_bt), tr_f);
			std::transform(bt.rbegin(), bt.rend(), std::back_inserter(c_bt), tr_f);

			Joint_ExceptionHandle joint_ex;
			Joint_Error ret = Joint_MakeException(msg.c_str(), c_bt.data(), c_bt.size(), &joint_ex);
			JOINT_CHECK(ret == JOINT_ERROR_NONE, std::string("Joint_MakeException failed: ") + Joint_ErrorToString(ret));
			outRetValue->releaseValue = &Object::ReleaseRetValue;
			outRetValue->result.ex = joint_ex;

			return JOINT_ERROR_EXCEPTION;
		}

		RetValueAllocator alloc;
		if (retType.id != JOINT_TYPE_VOID)
			outRetValue->result.value = ValueMarshaller::ToJoint(ValueDirection::Return, m_desc.GetRetType(), py_res, PythonMarshaller(), alloc);
		outRetValue->releaseValue = &Object::ReleaseRetValue;
		return JOINT_ERROR_NONE;
	}


	Joint_Error Object::ReleaseRetValue(Joint_Type type, Joint_Value value)
	{
		JOINT_CPP_WRAP_BEGIN
		switch(type.id)
		{
		case JOINT_TYPE_UTF8:
			delete[] value.utf8;
			break;
		case JOINT_TYPE_STRUCT:
			for (int32_t i = 0; i < type.payload.structDescriptor->membersCount; ++i)
				ReleaseRetValue(type.payload.structDescriptor->memberTypes[i], value.members[i]);
			delete[] value.members;
			break;
		default:
			break;
		}
		JOINT_CPP_WRAP_END
	}

}}
