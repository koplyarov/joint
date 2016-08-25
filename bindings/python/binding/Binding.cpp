#include <binding/Binding.hpp>

#include <joint/Joint.h>
#include <joint/devkit/Holder.hpp>
#include <joint/devkit/ScopeExit.hpp>
#include <joint/utils/CppWrappers.hpp>

#include <algorithm>
#include <memory>
#include <string.h>

#include <stdio.h>

#include <binding/Module.hpp>
#include <binding/Object.hpp>
#include <pyjoint/Common.hpp>
#include <pyjoint/Object.hpp>
#include <utils/PythonUtils.hpp>


namespace joint_python {
namespace binding
{

	Binding::Binding()
	{ GetLogger().Debug() << "Created"; }


	Binding::~Binding()
	{ GetLogger().Debug() << "Destroying"; }


	Joint_Error Binding::Deinit(void* bindingUserData)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<Binding*>(bindingUserData);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::LoadModule(void* bindingUserData, const char* moduleName, Joint_ModuleHandleInternal* outModule)
	{
		JOINT_CPP_WRAP_BEGIN
		*outModule = new Module(moduleName);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::UnloadModule(void* bindingUserData, Joint_ModuleHandleInternal module)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<Module*>(module);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::GetRootObject(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, const char* getterName, Joint_ObjectHandle* outObject)
	{
		JOINT_CPP_WRAP_BEGIN

		PyObjectHolder pyjoint_pymodule_name(PY_OBJ_CHECK(PyUnicode_FromString("pyjoint")));
		PyObjectHolder pyjoint_pymodule(PY_OBJ_CHECK_MSG(PyImport_Import(pyjoint_pymodule_name), "Could not import python module pyjoint"));
		PyObjectHolder pyjoint_module_type(PY_OBJ_CHECK(PyObject_GetAttrString(pyjoint_pymodule, "Module")));

		PyObjectHolder py_module_handle(PY_OBJ_CHECK(PyCapsule_New(module, "Joint.Module", NULL)));

		Joint_Error ret = Joint_IncRefModule(module);
		JOINT_CHECK(ret == JOINT_ERROR_NONE, std::string("Joint_IncRefModule failed: ") + Joint_ErrorToString(ret));
		joint::devkit::Holder<Joint_ModuleHandle> module_holder(module,
			[&](Joint_ModuleHandle h) { auto ret = Joint_DecRefModule(h); if(ret != JOINT_ERROR_NONE) GetLogger().Error() << "Joint_DecRefModule failed: " << ret; });

		PyObjectHolder py_params(PY_OBJ_CHECK(Py_BuildValue("(O)", py_module_handle.Get())));
		PyObjectHolder pyjoint_module(PY_OBJ_CHECK(PyObject_CallObject(pyjoint_module_type, py_params)));
		module_holder.Release();

		auto m = reinterpret_cast<Module*>(moduleInt);
		PyObjectHolder py_proxy = PY_OBJ_CHECK_MSG(m->InvokeFunction(getterName, pyjoint_module), joint::devkit::StringBuilder() % "Root object getter '" % getterName % "' failed");
		PyObjectHolder py_joint_obj(PY_OBJ_CHECK(PyObject_GetAttrString(py_proxy, "obj")));
		*outObject = CastPyObject<pyjoint::Object>(py_joint_obj, &pyjoint::Object_type)->handle;
		ret = Joint_IncRefObject(*outObject);
		JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::InvokeMethod(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Parameter* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN
		auto o = reinterpret_cast<Object*>(obj);
		PyObjectHolder py_res(o->InvokeMethod(methodId, joint::ArrayView<const Joint_Parameter>(params, paramsCount)));
		if (!py_res)
		{
			PyObjectHolder type, value, traceback;
			PyErr_Fetch(&type, &value, &traceback);
			//auto sg(joint::devkit::ScopeExit([&]{ PyErr_Restore(type, value, traceback); PyErr_Clear(); }));
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
			outRetValue->releaseValue = &Binding::ReleaseRetValue;
			outRetValue->result.ex = joint_ex;

			return JOINT_ERROR_EXCEPTION;
		}

		switch (retType.id)
		{
		case JOINT_TYPE_VOID:
			break;

		case JOINT_TYPE_BOOL: outRetValue->result.value.b = AsBool(py_res); break;

		case JOINT_TYPE_U8: outRetValue->result.value.u8 = FromPyLong<uint8_t>(py_res); break;
		case JOINT_TYPE_I8: outRetValue->result.value.i8 = FromPyLong<int8_t>(py_res); break;
		case JOINT_TYPE_U16: outRetValue->result.value.u16 = FromPyLong<uint16_t>(py_res); break;
		case JOINT_TYPE_I16: outRetValue->result.value.i16 = FromPyLong<int16_t>(py_res); break;
		case JOINT_TYPE_U32: outRetValue->result.value.u32 = FromPyLong<uint32_t>(py_res); break;
		case JOINT_TYPE_I32: outRetValue->result.value.i32 = FromPyLong<int32_t>(py_res); break;
		case JOINT_TYPE_U64: outRetValue->result.value.u64 = FromPyLong<uint64_t>(py_res); break;
		case JOINT_TYPE_I64: outRetValue->result.value.i64 = FromPyLong<int64_t>(py_res); break;

		case JOINT_TYPE_F32: outRetValue->result.value.f32 = FromPyFloat<float>(py_res); break;
		case JOINT_TYPE_F64: outRetValue->result.value.f64 = FromPyFloat<double>(py_res); break;

		case JOINT_TYPE_ENUM: outRetValue->result.value.e = FromPyLong<int32_t>(py_res); break;

		case JOINT_TYPE_UTF8:
			{
				auto str_data = Utf8FromPyUnicode(py_res);
				std::unique_ptr<char[]> result_str(new char[strlen(str_data.GetContent()) + 1]);
#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable: 4996)
#endif
				strcpy(result_str.get(), str_data.GetContent());
#ifdef _MSC_VER
#	pragma warning(pop)
#endif
				outRetValue->result.value.utf8 = result_str.release();
			}
			break;
		case JOINT_TYPE_OBJ:
			{
				if (py_res.Get() != Py_None)
				{
					PyObjectHolder py_proxy_type(PY_OBJ_CHECK(PyObject_Type(py_res)));
					PyObjectHolder py_checksum(PY_OBJ_CHECK(PyObject_GetAttrString(py_proxy_type, "interfaceChecksum")));
					if (FromPyLong<Joint_InterfaceChecksum>(py_checksum) != retType.payload.interfaceChecksum)
						JOINT_THROW(JOINT_ERROR_INVALID_INTERFACE_CHECKSUM);

					PyObjectHolder py_joint_obj(PY_OBJ_CHECK(PyObject_GetAttrString(py_res, "obj")));
					outRetValue->result.value.obj = CastPyObject<pyjoint::Object>(py_joint_obj, &pyjoint::Object_type)->handle;
					Joint_Error ret = Joint_IncRefObject(outRetValue->result.value.obj);
					JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
				}
				else
					outRetValue->result.value.obj = JOINT_NULL_HANDLE;
			}
			break;
		default:
			JOINT_THROW(std::runtime_error("Unknown type"));
		}

		outRetValue->releaseValue = &Binding::ReleaseRetValue;

		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::ReleaseObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj)
	{
		JOINT_CPP_WRAP_BEGIN

		delete reinterpret_cast<Object*>(obj);

		JOINT_CPP_WRAP_END
	}


	PyObjectHolder Binding::FindBaseById(PyObject* type, const char* interfaceId)
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

	Joint_Error Binding::CastObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, Joint_InterfaceId interfaceId, Joint_InterfaceChecksum checksum, Joint_ObjectHandleInternal* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN

		PyObjectHolder py_accessor = reinterpret_cast<Object*>(obj)->GetObject();
		PyObjectHolder py_obj(PY_OBJ_CHECK(PyObject_GetAttrString(py_accessor, "obj")));
		PyObjectHolder py_obj_type(PyObject_Type(py_obj));
		PyObjectHolder base_type = FindBaseById(py_obj_type, interfaceId);
		if (!base_type)
			return JOINT_ERROR_CAST_FAILED;

		PyObjectHolder py_checksum(PY_OBJ_CHECK(PyObject_GetAttrString(base_type, "interfaceChecksum")));
		if (FromPyLong<Joint_InterfaceChecksum>(py_checksum) != checksum)
			return JOINT_ERROR_INVALID_INTERFACE_CHECKSUM;

		PyObjectHolder base_accessor_type(PY_OBJ_CHECK(PyObject_GetAttrString(base_type, "accessor")));

		PyObjectHolder py_params(Py_BuildValue("(O)", (PyObject*)py_obj));
		PyObjectHolder new_accessor(PY_OBJ_CHECK(PyObject_CallObject(base_accessor_type, py_params)));
		*outRetValue = new Object(new_accessor);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::ReleaseRetValue(Joint_Type type, Joint_Value value)
	{
		JOINT_CPP_WRAP_BEGIN
		switch(type.id)
		{
		case JOINT_TYPE_UTF8:
			delete[] value.utf8;
			break;
		case JOINT_TYPE_OBJ:
			break;
		default:
			JOINT_THROW(JOINT_ERROR_GENERIC);
			break;
		}
		JOINT_CPP_WRAP_END
	}

}}
