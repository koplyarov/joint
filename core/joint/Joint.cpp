#include <joint/Joint.h>

#include <joint/core/JointCore.hpp>
#include <joint/utils/CppWrappers.hpp>
#include <joint/utils/JointException.hpp>
#include <joint/utils/MakeUnique.hpp>

#include <algorithm>
#include <atomic>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string.h>
#include <vector>

#include <stdarg.h>


static void DefaultLogCallback(const char* message)
{ printf("%s\n", message); }


static joint::JointCore               g_core;
static std::mutex                     g_mutex;
static Joint_LogCallback_Func*        g_logCallback = &DefaultLogCallback;
static std::atomic<Joint_LogLevel>    g_logLevel(JOINT_LOGLEVEL_DEBUG);

extern "C"
{

	Joint_Error Joint_SetLogCallback(Joint_LogCallback_Func* logCallback)
	{
		JOINT_CPP_WRAP_BEGIN
		std::lock_guard<std::mutex> l(g_mutex);
		g_logCallback = (logCallback == nullptr) ? &DefaultLogCallback : logCallback;
		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_SetLogLevel(Joint_LogLevel logLevel)
	{
		JOINT_CPP_WRAP_BEGIN
		JOINT_CHECK(logLevel >= JOINT_LOGLEVEL_DEBUG && logLevel <= JOINT_LOGLEVEL_ERROR, JOINT_ERROR_INVALID_PARAMETER);
		g_logLevel = logLevel;
		JOINT_CPP_WRAP_END
	}


	void Joint_Log(Joint_LogLevel logLevel, const char* subsystem, const char* format, ...)
	{
		if (logLevel < g_logLevel.load(std::memory_order_relaxed))
			return;

		char message[1024];
		int ofs = 0;

		const char* log_level_str = "UNKNOWN LOG LEVEL";
		switch (logLevel)
		{
		case JOINT_LOGLEVEL_DEBUG:    log_level_str = "Debug";    break;
		case JOINT_LOGLEVEL_INFO:     log_level_str = "Info";     break;
		case JOINT_LOGLEVEL_WARNING:  log_level_str = "Warning";  break;
		case JOINT_LOGLEVEL_ERROR:    log_level_str = "Error";    break;
		}

		ofs += snprintf(&message[ofs], sizeof(message) - ofs, "[%s] [%s] ", log_level_str, subsystem);

		va_list argptr;
		va_start(argptr, format);
		vsnprintf(&message[ofs], sizeof(message), format, argptr);
		va_end(argptr);

		std::lock_guard<std::mutex> l(g_mutex);
		g_logCallback(message);
	}


	const char* Joint_ErrorToString(Joint_Error err)
	{
		switch (err)
		{
		case JOINT_ERROR_NONE:                    return "JOINT_ERROR_NONE";
		case JOINT_ERROR_GENERIC:                 return "JOINT_ERROR_GENERIC ";
		case JOINT_ERROR_NOT_IMPLEMENTED:         return "JOINT_ERROR_NOT_IMPLEMENTED";
		case JOINT_ERROR_INVALID_PARAMETER:       return "JOINT_ERROR_INVALID_PARAMETER";
		case JOINT_ERROR_OUT_OF_MEMORY:           return "JOINT_ERROR_OUT_OF_MEMORY ";
		case JOINT_ERROR_IMPLEMENTATION_ERROR:    return "JOINT_ERROR_IMPLEMENTATION_ERROR";
		case JOINT_ERROR_UNEXPECTED_TYPE:         return "JOINT_ERROR_UNEXPECTED_TYPE";
		}

		return "Unknown error";
	}


	Joint_Error Joint_RegisterBinding(Joint_BindingDesc desc, void* userData, Joint_BindingHandle* outBinding)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(outBinding, JOINT_ERROR_INVALID_PARAMETER);

		*outBinding = g_core.RegisterBinding(desc, userData);
		Joint_Log(JOINT_LOGLEVEL_INFO, "Joint", "RegisterBinding(name: \"%s\"): %p", desc.name, *outBinding);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_UnregisterBinding(Joint_BindingHandle handle)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle, JOINT_ERROR_INVALID_PARAMETER);

		Joint_Log(JOINT_LOGLEVEL_INFO, "Joint", "UnregisterBinding(binding: %p)", handle);
		g_core.UnregisterBinding(handle);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_LoadModule(const char* bindingName, const char* moduleName, Joint_ModuleHandle* outModule)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(bindingName, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(moduleName, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outModule, JOINT_ERROR_INVALID_PARAMETER);

		*outModule = g_core.LoadModule(bindingName, moduleName);
		Joint_Log(JOINT_LOGLEVEL_INFO, "Joint", "LoadModule(bindingName: \"%s\", moduleName: \"%s\"): %p", bindingName, moduleName, *outModule);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_UnloadModule(Joint_ModuleHandle handle)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle, JOINT_ERROR_INVALID_PARAMETER);

		Joint_Log(JOINT_LOGLEVEL_INFO, "Joint", "UnloadModule(module: %p)", handle);
		g_core.UnloadModule(handle);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_GetRootObject(Joint_ModuleHandle module, const char* getterName, Joint_ObjectHandle* outObject)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(module, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(getterName, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outObject, JOINT_ERROR_INVALID_PARAMETER);

		Joint_ObjectHandleInternal internal;
		Joint_Error ret = module->binding->desc.getRootObject(module->binding->userData, module->internal, getterName, &internal);
		JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
		JOINT_CHECK(internal, JOINT_ERROR_IMPLEMENTATION_ERROR);
		*outObject = new Joint_Object{ internal, module };
		Joint_Log(JOINT_LOGLEVEL_DEBUG, "Joint", "GetRootObject(module: %p, getterName: \"%s\"): %p", module, getterName, *outObject);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_InvokeMethod(Joint_ObjectHandle obj, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(obj, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(params || paramsCount == 0, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outRetValue, JOINT_ERROR_INVALID_PARAMETER);

		Joint_RetValueInternal ret_value_internal;
		Joint_Error ret = obj->module->binding->desc.invokeMethod(obj->module->binding->userData, obj->module->internal, obj->internal, methodId, params, paramsCount, retType, &ret_value_internal);
		JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);

		JOINT_CHECK(ret_value_internal.releaseValue, JOINT_ERROR_IMPLEMENTATION_ERROR);

		static_assert(sizeof(outRetValue->variant) == sizeof(outRetValue->internal.variant) && alignof(outRetValue->variant) == alignof(outRetValue->internal.variant), "Sizes or alignments do not match for Joint_Variant and Joint_VariantInternal");
		memcpy(&outRetValue->internal, &ret_value_internal, sizeof(outRetValue->internal));
		memcpy(&outRetValue->variant, &outRetValue->internal.variant, sizeof(outRetValue->variant));
		if (outRetValue->internal.variant.type == JOINT_TYPE_OBJ)
			outRetValue->variant.value.obj = new Joint_Object{ outRetValue->internal.variant.value.obj, obj->module };

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_ObtainRetValue(Joint_RetValue value, Joint_Variant* outValue)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(outValue, JOINT_ERROR_INVALID_PARAMETER);

		*outValue = value.variant;

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_ReleaseRetValue(Joint_RetValue value)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(value.internal.releaseValue, JOINT_ERROR_INVALID_PARAMETER);

		return value.internal.releaseValue(value.internal.variant);

		JOINT_CPP_WRAP_END
	}

}
