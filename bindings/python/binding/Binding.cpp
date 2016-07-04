#include <binding/Binding.hpp>

#include <joint/Joint.h>
#include <joint/utils/CppWrappers.hpp>

#include <memory>
#include <string.h>

#include <stdio.h>

#include <binding/Module.hpp>
#include <binding/Object.hpp>
#include <pyjoint/Common.hpp>
#include <utils/PythonUtils.hpp>


namespace joint_python {
namespace binding
{

	Binding::Binding()
	{
		Joint_Log(JOINT_LOGLEVEL_DEBUG, "Binding", "Created Binding");
	}


	Binding::~Binding()
	{
		Joint_Log(JOINT_LOGLEVEL_DEBUG, "Binding", "Destroyed Binding");
	}


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


	Joint_Error Binding::GetRootObject(void* bindingUserData, Joint_ModuleHandleInternal module, const char* getterName, Joint_ObjectHandleInternal* outObject)
	{
		JOINT_CPP_WRAP_BEGIN
		auto m = reinterpret_cast<Module*>(module);
		PyObjectHolder py_obj = m->InvokeFunction(getterName);
		*outObject = new Object(py_obj);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::InvokeMethod(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValueInternal* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN
		auto o = reinterpret_cast<Object*>(obj);
		auto py_res = o->InvokeMethod(methodId, joint::ArrayView<const Joint_Variant>(params, paramsCount));

		switch (retType)
		{
		case JOINT_TYPE_VOID:
			break;
		case JOINT_TYPE_I32:
			outRetValue->variant.value.i32 = FromPyLong<int32_t>(py_res);
			break;
		case JOINT_TYPE_UTF8:
			{
				auto str_data = Utf8FromPyUnicode(py_res);
				std::unique_ptr<char[]> result_str(new char[strlen(str_data.GetContent())]);
				strcpy(result_str.get(), str_data.GetContent());
				outRetValue->variant.value.utf8 = result_str.release();
			}
			break;
		case JOINT_TYPE_OBJ:
			outRetValue->variant.value.obj = new Object(py_res);
			break;
		default:
			JOINT_THROW(std::runtime_error("Unknown type"));
		}

		outRetValue->variant.type = retType;
		outRetValue->releaseValue = &Binding::ReleaseRetValue;

		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::ReleaseObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj)
	{
		JOINT_CPP_WRAP_BEGIN

		delete reinterpret_cast<Object*>(obj);

		JOINT_CPP_WRAP_END
	}


	static PyObjectHolder FindBaseById(PyObject* type, const char* interfaceId)
	{
		PyObjectHolder bases(PyObject_GetAttrString(type, "__bases__"));
		PyObjectHolder seq(PySequence_Fast((PyObject*)bases, "A sequence expected!"));
		int len = PySequence_Size(seq);
		for (int i = 0; i < len; ++i)
		{
			PyObject* base = PySequence_Fast_GET_ITEM(seq.Get(), i);
			JOINT_CHECK(base, "None base class???");

			if (!PyObject_HasAttrString(base, "interfaceId"))
				continue;

#if PY_VERSION_HEX >= 0x03000000
			PyObjectHolder py_base_interface_id(PyObject_GetAttrString(base, "interfaceId"));
#else
			PyObjectHolder py_base_interface_id_attr(PyObject_GetAttrString(base, "interfaceId"));
			PyObjectHolder py_base_interface_id(PyObject_Unicode(py_base_interface_id_attr));
#endif
			JOINT_CHECK(py_base_interface_id, "No interfaceId attribute");

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

	Joint_Error Binding::CastObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, Joint_InterfaceId interfaceId, Joint_ObjectHandleInternal* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN

		PyObjectHolder py_accessor = reinterpret_cast<Object*>(obj)->GetObject();
		PyObjectHolder py_obj(PyObject_GetAttrString(py_accessor, "obj"));
		JOINT_CHECK(py_obj, "No obj attribute!");
		PyObjectHolder py_obj_type(PyObject_Type(py_obj));
		PyObjectHolder base_type = FindBaseById(py_obj_type, interfaceId);
		JOINT_CHECK(base_type, std::string("Could not cast object to ") + interfaceId);

		PyObjectHolder base_accessor_type(PyObject_GetAttrString(base_type, "accessor"));
		JOINT_CHECK(base_accessor_type, "No accessor attribute");

		PyObjectHolder new_accessor(PyObject_CallObject(base_accessor_type, Py_BuildValue("(O)", (PyObject*)py_obj)));
		JOINT_CHECK(new_accessor, "Could not wrap object");
		*outRetValue = new Object(new_accessor);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::ReleaseRetValue(Joint_VariantInternal value)
	{
		JOINT_CPP_WRAP_BEGIN
		switch(value.type)
		{
		case JOINT_TYPE_VOID:
		case JOINT_TYPE_BOOL:
		case JOINT_TYPE_I8:
		case JOINT_TYPE_U8:
		case JOINT_TYPE_I16:
		case JOINT_TYPE_U16:
		case JOINT_TYPE_I32:
		case JOINT_TYPE_U32:
		case JOINT_TYPE_I64:
		case JOINT_TYPE_U64:
		case JOINT_TYPE_F32:
		case JOINT_TYPE_F64:
			break;
		case JOINT_TYPE_UTF8:
			delete[] value.value.utf8;
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
