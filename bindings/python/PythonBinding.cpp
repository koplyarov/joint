#include "PythonBinding.h"

#include <joint/Joint.h>
#include <joint/utils/CppWrappers.hpp>

#include <memory>
#include <string.h>

#include <stdio.h>

#include "PythonContext.hpp"
#include "PythonModule.hpp"
#include "PythonObject.hpp"


class PythonBinding
{
private:
	PythonContext	_pyCtx;

public:
	PythonBinding()
	{
		Joint_Log(JOINT_LOGLEVEL_DEBUG, "PythonBinding", "Created PythonBinding");
	}

	~PythonBinding()
	{
		Joint_Log(JOINT_LOGLEVEL_DEBUG, "PythonBinding", "Destroyed PythonBinding");
	}

	static Joint_Error Deinit(void* bindingUserData)
	{
		reinterpret_cast<PythonBinding*>(bindingUserData)->~PythonBinding();
		return JOINT_ERROR_NONE;
	}

	static Joint_Error LoadModule(void* bindingUserData, const char* moduleName, Joint_ModuleHandleInternal* outModule)
	{
		JOINT_CPP_WRAP_BEGIN
		*outModule = new PythonModule(moduleName);
		JOINT_CPP_WRAP_END
	}

	static Joint_Error UnloadModule(void* bindingUserData, Joint_ModuleHandleInternal module)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<PythonModule*>(module);
		JOINT_CPP_WRAP_END
	}

	static Joint_Error GetRootObject(void* bindingUserData, Joint_ModuleHandleInternal module, const char* getterName, Joint_ObjectHandleInternal* outObject)
	{
		JOINT_CPP_WRAP_BEGIN
		auto m = reinterpret_cast<PythonModule*>(module);
		PyObjectPtr py_obj = m->InvokeFunction(getterName);
		*outObject = new PythonObject(py_obj);
		JOINT_CPP_WRAP_END
	}

	static Joint_Error InvokeMethod(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN
		auto o = reinterpret_cast<PythonObject*>(obj);
		auto py_res = o->InvokeMethod(methodId, joint::ArrayView<const Joint_Variant>(params, paramsCount));

		switch (retType)
		{
		case JOINT_TYPE_VOID:
			break;
		case JOINT_TYPE_I32:
			{
				int overflow = 0;
				long result = PyLong_AsLongAndOverflow(py_res, &overflow);
				if (overflow != 0)
					PYTHON_ERROR("Overflow in PyLong_AsLongAndOverflow");
				 outRetValue->variant.value.i32 = result;
			}
			break;
		case JOINT_TYPE_UTF8:
			{
				PyObjectPtr py_bytes(PyUnicode_AsUTF8String(py_res));
				if (!py_bytes)
					PYTHON_ERROR("PyUnicode_AsUTF8String failed!");
				const char* str_data = PyBytes_AsString(py_bytes);
				if (!str_data)
					PYTHON_ERROR("PyBytes_AsString failed!");
				char* result_str = new char[strlen(str_data)];
				strcpy(result_str, str_data);
				outRetValue->variant.value.utf8 = result_str;
			}
			break;
		case JOINT_TYPE_OBJ:
			JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED);
			break;
		default:
			JOINT_THROW(std::runtime_error("Unknown type"));
		}

		outRetValue->variant.type = retType;
		outRetValue->releaseValue = &PythonBinding::ReleaseRetValue;

		JOINT_CPP_WRAP_END
	}

	static Joint_Error ReleaseRetValue(Joint_Variant value)
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
			JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); // TODO: ???
			break;
		default:
			JOINT_THROW(JOINT_ERROR_GENERIC);
			break;
		}
		JOINT_CPP_WRAP_END
	}
};


static Joint_BindingHandle g_bindingHandle = JOINT_NULL_HANDLE;


extern "C"
{

	Joint_Error JointPython_Register()
	{
		Joint_BindingDesc binding_desc = { };
		binding_desc.name            = "python";
		binding_desc.deinitBinding   = &PythonBinding::Deinit;
		binding_desc.loadModule      = &PythonBinding::LoadModule;
		binding_desc.unloadModule    = &PythonBinding::UnloadModule;
		binding_desc.getRootObject   = &PythonBinding::GetRootObject;
		binding_desc.invokeMethod    = &PythonBinding::InvokeMethod;

		std::unique_ptr<PythonBinding> binding(new PythonBinding);
		Joint_Error ret = Joint_RegisterBinding(binding_desc, binding.get(), &g_bindingHandle);
		if (ret != JOINT_ERROR_NONE)
			Joint_Log(JOINT_LOGLEVEL_ERROR, "PythonBinding", "Joint_RegisterBinding failed: %s", Joint_ErrorToString(ret));

		binding.release();
		return ret;
	}


	Joint_Error JointPython_Unregister()
	{
		if (g_bindingHandle == JOINT_NULL_HANDLE)
			return JOINT_ERROR_NONE;

		Joint_Error ret = Joint_UnregisterBinding(g_bindingHandle);
		g_bindingHandle = JOINT_NULL_HANDLE;
		if (ret != JOINT_ERROR_NONE)
			Joint_Log(JOINT_LOGLEVEL_WARNING, "PythonBinding", "Joint_UnregisterBinding failed: %s", Joint_ErrorToString(ret));

		return ret;
	}

}
