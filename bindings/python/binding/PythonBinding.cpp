#include <binding/PythonBinding.hpp>

#include <joint/Joint.h>
#include <joint/utils/CppWrappers.hpp>

#include <memory>
#include <string.h>

#include <stdio.h>

#include <binding/PythonModule.hpp>
#include <binding/PythonObject.hpp>


PythonBinding::PythonBinding()
{
	Joint_Log(JOINT_LOGLEVEL_DEBUG, "PythonBinding", "Created PythonBinding");
}


PythonBinding::~PythonBinding()
{
	Joint_Log(JOINT_LOGLEVEL_DEBUG, "PythonBinding", "Destroyed PythonBinding");
}


Joint_Error PythonBinding::Deinit(void* bindingUserData)
{
	JOINT_CPP_WRAP_BEGIN
	reinterpret_cast<PythonBinding*>(bindingUserData)->~PythonBinding();
	JOINT_CPP_WRAP_END
}


Joint_Error PythonBinding::LoadModule(void* bindingUserData, const char* moduleName, Joint_ModuleHandleInternal* outModule)
{
	JOINT_CPP_WRAP_BEGIN
	*outModule = new PythonModule(moduleName);
	JOINT_CPP_WRAP_END
}


Joint_Error PythonBinding::UnloadModule(void* bindingUserData, Joint_ModuleHandleInternal module)
{
	JOINT_CPP_WRAP_BEGIN
	delete reinterpret_cast<PythonModule*>(module);
	JOINT_CPP_WRAP_END
}


Joint_Error PythonBinding::GetRootObject(void* bindingUserData, Joint_ModuleHandleInternal module, const char* getterName, Joint_ObjectHandleInternal* outObject)
{
	JOINT_CPP_WRAP_BEGIN
	auto m = reinterpret_cast<PythonModule*>(module);
	PyObjectPtr py_obj = m->InvokeFunction(getterName);
	*outObject = new PythonObject(py_obj);
	JOINT_CPP_WRAP_END
}


Joint_Error PythonBinding::InvokeMethod(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValueInternal* outRetValue)
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
		outRetValue->variant.value.obj = new PythonObject(py_res);
		break;
	default:
		JOINT_THROW(std::runtime_error("Unknown type"));
	}

	outRetValue->variant.type = retType;
	outRetValue->releaseValue = &PythonBinding::ReleaseRetValue;

	JOINT_CPP_WRAP_END
}


Joint_Error PythonBinding::ReleaseObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj)
{
	JOINT_CPP_WRAP_BEGIN

	delete reinterpret_cast<PythonObject*>(obj);

	JOINT_CPP_WRAP_END
}


Joint_Error PythonBinding::ReleaseRetValue(Joint_VariantInternal value)
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
