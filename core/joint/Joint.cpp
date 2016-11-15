#include <joint/JointConfig.h>
#include <joint/devkit/Logger.hpp>
#include <joint/private/JointStructs.h>
#include <joint/devkit/CppWrappers.hpp>
#include <joint/devkit/JointException.hpp>
#include <joint/devkit/MakeUnique.hpp>

#include <algorithm>
#include <atomic>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string.h>
#include <vector>

#include <stdarg.h>


#if JOINT_PLATFORM_POSIX
#	include <dlfcn.h>
#	include <unistd.h>
#endif


static const char* LoggerName = "Joint.Core";
JOINT_DEVKIT_LOGGER(LoggerName)


static void DefaultLogCallback(Joint_LogLevel logLevel, const char* subsystem, const char* message)
{
	const char* alignment = "        ";
	const char* log_level_str = Joint_LogLevelToString(logLevel);
#if JOINT_PLATFORM_POSIX
	if (isatty(2))
	{
		const char* subsystem_ns_color = "\33[0;32m";
		const char* subsystem_name_color = "\33[1;32m";
		const char* log_color = "\33[0m";

		switch (logLevel)
		{
		case JOINT_LOGLEVEL_DEBUG:    log_color = "\33[2;37m";  break;
		case JOINT_LOGLEVEL_INFO:     log_color = "\33[0m";     break;
		case JOINT_LOGLEVEL_WARNING:  log_color = "\33[1;33m";  break;
		case JOINT_LOGLEVEL_ERROR:    log_color = "\33[1;31m";  break;
		}

		int subsystem_len = strlen(subsystem), subsystem_name_pos = subsystem_len;
		while (subsystem_name_pos > 0 && subsystem[subsystem_name_pos] != '.')
			--subsystem_name_pos;
		if (subsystem[subsystem_name_pos] == '.')
			++subsystem_name_pos;

		fprintf(stderr, "%s[%s]%.*s%s[%.*s%s%s%s] %s%s\33[0m\n",
			log_color, log_level_str, (int)(sizeof("Warning") - strlen(log_level_str)), alignment,
			subsystem_ns_color, subsystem_name_pos, subsystem, subsystem_name_color, subsystem + subsystem_name_pos, subsystem_ns_color,
			log_color, message);
	}
	else
#endif
		fprintf(stderr, "[%s]%.*s[%s] %s\n", Joint_LogLevelToString(logLevel), (int)(sizeof("Warning") - strlen(log_level_str)), alignment, subsystem, message);
}


static std::mutex                     g_mutex;
static Joint_LogCallback_Func*        g_logCallback = &DefaultLogCallback;
//static std::atomic<Joint_LogLevel>    g_logLevel(JOINT_LOGLEVEL_DEBUG);
static std::atomic<Joint_LogLevel>    g_logLevel(JOINT_LOGLEVEL_WARNING);

extern "C"
{

	const char* Joint_LogLevelToString(Joint_LogLevel logLevel)
	{
		switch (logLevel)
		{
		case JOINT_LOGLEVEL_DEBUG:    return "Debug";
		case JOINT_LOGLEVEL_INFO:     return "Info";
		case JOINT_LOGLEVEL_WARNING:  return "Warning";
		case JOINT_LOGLEVEL_ERROR:    return "Error";
		default:                      return "Unknown log level";
		}
	}

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


	Joint_LogLevel Joint_GetLogLevel()
	{ return g_logLevel.load(std::memory_order_relaxed); }


	void Joint_Log(Joint_LogLevel logLevel, const char* subsystem, const char* format, ...)
	{
		if (logLevel < g_logLevel.load(std::memory_order_relaxed))
			return;

		char message[1024];
		va_list argptr;
		va_start(argptr, format);
		vsnprintf(message, sizeof(message), format, argptr);
		va_end(argptr);

		std::lock_guard<std::mutex> l(g_mutex);
		g_logCallback(logLevel, subsystem, message);
	}


	const char* Joint_ErrorToString(Joint_Error err)
	{
#define DETAIL_JOINT_ERR_TO_STRING(Val_) case Val_: return #Val_
		switch (err)
		{
		DETAIL_JOINT_ERR_TO_STRING(JOINT_ERROR_NONE);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_ERROR_CAST_FAILED);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_ERROR_EXCEPTION);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_ERROR_GENERIC);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_ERROR_NO_SUCH_BINDING);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_ERROR_NO_SUCH_MODULE);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_ERROR_NOT_IMPLEMENTED);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_ERROR_INVALID_PARAMETER);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_ERROR_OUT_OF_MEMORY);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_ERROR_IMPLEMENTATION_ERROR);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_ERROR_INVALID_INTERFACE_CHECKSUM);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_ERROR_INDEX_OUT_OF_RANGE);
		}
#undef DETAIL_JOINT_ERR_TO_STRING

		return "Unknown error";
	}


	Joint_Error Joint_MakeContext(Joint_ContextHandle *outJointCtx)
	{
		JOINT_CPP_WRAP_BEGIN

		*outJointCtx = new Joint_Context;

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_ReleaseContext(Joint_ContextHandle jointCtx)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(jointCtx != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);

		delete jointCtx;

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_MakeBinding(Joint_BindingDesc desc, void* userData, Joint_BindingHandle* outBinding)
	{
		JOINT_CPP_WRAP_BEGIN

		GetLogger().Info() << "MakeBinding(name: " << desc.name << ")";

		JOINT_CHECK(outBinding, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(desc.name != nullptr, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(desc.invokeMethod != nullptr, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(desc.releaseObject != nullptr, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(desc.castObject != nullptr, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(desc.getRootObject != nullptr, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(desc.loadModule != nullptr, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(desc.unloadModule != nullptr, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(desc.deinitBinding != nullptr, JOINT_ERROR_INVALID_PARAMETER);

		*outBinding = new Joint_Binding(userData, desc);

		GetLogger().Debug() << "MakeBinding.outBinding: " << (void*)*outBinding;

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_IncRefBinding(Joint_BindingHandle handle)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		++handle->refCount;
		GetLogger().Debug() << "IncRefBinding(binding: " << handle << " (userData: " << (handle ? handle->userData : NULL) << ")" << "): " << handle->refCount;

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_DecRefBinding(Joint_BindingHandle handle)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);

		auto refs = --handle->refCount;
		GetLogger().Debug() << "DecRefBinding(binding: " << handle << " (userData: " << (handle ? handle->userData : NULL) << ")" << "): " << handle->refCount;
		if (refs == 0)
		{
			GetLogger().Info() << "ReleaseBinding(binding: " << handle << " (userData: " << (handle ? handle->userData : NULL) << ")" << ")";
			Joint_Error ret = handle->desc.deinitBinding(handle->userData);
			delete handle;
			JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
		}

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_LoadModule(Joint_BindingHandle binding, const char* moduleName, Joint_ModuleHandle* outModule)
	{
		JOINT_CPP_WRAP_BEGIN

		GetLogger().Info() << "LoadModule(binding: " << binding << " (userData: " << (binding ? binding->userData : NULL) << "), moduleName: \"" << moduleName << "\")";

		JOINT_CHECK(binding != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(moduleName, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outModule, JOINT_ERROR_INVALID_PARAMETER);

		Joint_ModuleHandleInternal internal = JOINT_NULL_HANDLE;
		Joint_Error ret = binding->desc.loadModule(binding->userData, moduleName, &internal);
		JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
		JOINT_CHECK(internal, JOINT_ERROR_IMPLEMENTATION_ERROR);

		*outModule = new Joint_Module(internal, binding);

		GetLogger().Debug() << "  LoadModule.outModule: " << *outModule;

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_MakeModule(Joint_BindingHandle binding, Joint_ModuleHandleInternal internal, Joint_ModuleHandle* outModule)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(outModule, JOINT_ERROR_INVALID_PARAMETER);
		*outModule = new Joint_Module(internal, binding);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_IncRefModule(Joint_ModuleHandle handle)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		++handle->refCount;

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_DecRefModule(Joint_ModuleHandle handle)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		auto refs = --handle->refCount;
		if (refs == 0)
		{
			GetLogger().Info() << "UnloadModule(module: " << handle << ")";
			Joint_Error ret = handle->binding->desc.unloadModule(handle->binding->userData, handle->internal);
			delete handle;
			JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
		}
		JOINT_CHECK(refs >= 0, JOINT_ERROR_INVALID_PARAMETER);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_CreateObject(Joint_ModuleHandle module, Joint_ObjectHandleInternal internal, Joint_ObjectHandle* outObject)
	{
		JOINT_CPP_WRAP_BEGIN

		*outObject = new Joint_Object(internal, module);
		return Joint_IncRefModule(module);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_GetRootObject(Joint_ModuleHandle module, const char* getterName, Joint_ObjectHandle* outObject)
	{
		JOINT_CPP_WRAP_BEGIN

		GetLogger().Debug() << "GetRootObject(module: " << module << ", getterName: \"" << getterName << "\")";

		JOINT_CHECK(module != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(getterName, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outObject, JOINT_ERROR_INVALID_PARAMETER);

		Joint_Error ret = module->binding->desc.getRootObject(module, module->binding->userData, module->internal, getterName, outObject);
		JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);

		GetLogger().Debug() << "  GetRootObject.outObject: " << *outObject << " (internal: " << (*outObject)->internal << ")";

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_InvokeMethod(Joint_ObjectHandle obj, Joint_SizeT methodId, const Joint_Parameter* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
	{
		// Nothing here may throw, so there is no JOINT_CPP_WRAP macros. This improves performance

		JOINT_CHECK_NOTHROW(obj != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK_NOTHROW(obj->refCount.load(std::memory_order_relaxed) > 0, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK_NOTHROW(params || paramsCount == 0, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK_NOTHROW(outRetValue, JOINT_ERROR_INVALID_PARAMETER);

		Joint_Error ret = obj->module->binding->desc.invokeMethod(obj->module, obj->module->binding->userData, obj->module->internal, obj->internal, methodId, params, paramsCount, retType, outRetValue);
		JOINT_CHECK_NOTHROW(ret == JOINT_ERROR_NONE || ret == JOINT_ERROR_EXCEPTION, ret);
		JOINT_CHECK_NOTHROW(outRetValue->releaseValue, JOINT_ERROR_IMPLEMENTATION_ERROR);

		return ret;
	}


	void Joint_IncRefObject(Joint_ObjectHandle handle)
	{
		if (handle == JOINT_NULL_HANDLE)
			return;

		if (++handle->refCount <= 1)
			JOINT_TERMINATE("Inconsistent reference counter!");
	}


	void Joint_DecRefObject(Joint_ObjectHandle handle)
	{
		if (handle == JOINT_NULL_HANDLE)
			return;

		auto refs = --handle->refCount;
		if (refs < 0)
			JOINT_TERMINATE("Inconsistent reference counter!");

		if (refs == 0)
		{
			JOINT_CPP_WRAP_BEGIN

			GetLogger().Debug() << "ReleaseObject(obj: " << handle << " (internal: " << (handle ? handle->internal : NULL) << "))";

			Joint_Error ret = handle->module->binding->desc.releaseObject(handle->module->binding->userData, handle->module->internal, handle->internal);
			if (ret != JOINT_ERROR_NONE)
				GetLogger().Error() << "releaseObject failed: " << ret;

			ret = Joint_DecRefModule(handle->module);
			if (ret != JOINT_ERROR_NONE)
				GetLogger().Error() << "Joint_DecRefModule failed: " << ret;

			delete handle;

			JOINT_CPP_WRAP_END_VOID
		}
	}


	Joint_Error Joint_CastObject(Joint_ObjectHandle handle, Joint_InterfaceId interfaceId, Joint_InterfaceChecksum checksum, Joint_ObjectHandle* outHandle)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(handle->refCount.load(std::memory_order_relaxed) > 0, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(interfaceId, JOINT_ERROR_INVALID_PARAMETER);

		Joint_ObjectHandleInternal internal = NULL;
		Joint_Error ret = handle->module->binding->desc.castObject(handle->module->binding->userData, handle->module->internal, handle->internal, interfaceId, checksum, &internal);
		if (ret != JOINT_ERROR_NONE)
		{
			if (ret == JOINT_ERROR_CAST_FAILED)
			{
				*outHandle = JOINT_NULL_HANDLE;
				return ret;
			}

			JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
		}

		JOINT_CHECK(internal, JOINT_ERROR_IMPLEMENTATION_ERROR);
		*outHandle = new Joint_Object(internal, handle->module);

		return Joint_IncRefModule(handle->module);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_MakeException(const char* message, const Joint_StackFrame* backtrace, Joint_SizeT backtraceSize, Joint_ExceptionHandle* outHandle)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(outHandle, JOINT_ERROR_INVALID_PARAMETER);

		std::vector<Joint_StackFrameData> bt;
		bt.reserve(backtraceSize);
		for (Joint_SizeT i = 0; i < backtraceSize; ++i)
		{
			auto sf = backtrace[i];
			bt.push_back(Joint_StackFrameData{sf.module, sf.filename, sf.line, sf.code, sf.function});
		}
		*outHandle = new Joint_Exception{message, bt};

		JOINT_CPP_WRAP_END
	}


	void Joint_ReleaseException(Joint_ExceptionHandle handle)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);

		delete handle;

		JOINT_CPP_WRAP_END_VOID
	}


	Joint_Error Joint_GetExceptionMessageSize(Joint_ExceptionHandle handle, Joint_SizeT* outSize)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outSize, JOINT_ERROR_INVALID_PARAMETER);

		*outSize = handle->message.size() + 1;

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_GetExceptionMessage(Joint_ExceptionHandle handle, char* buf, Joint_SizeT bufSize)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(buf, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(bufSize >= handle->message.size() + 1, JOINT_ERROR_INVALID_PARAMETER);

		strcpy(buf, handle->message.c_str());

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_GetExceptionBacktraceSize(Joint_ExceptionHandle handle, Joint_SizeT* outSize)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outSize, JOINT_ERROR_INVALID_PARAMETER);

		*outSize = handle->backtrace.size();

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_GetExceptionBacktraceEntry(Joint_ExceptionHandle handle, Joint_SizeT index, Joint_StackFrame* outStackFrame)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(index < handle->backtrace.size(), JOINT_ERROR_INVALID_PARAMETER);

		const auto& sf = handle->backtrace[index];
		outStackFrame->module = sf.module.c_str();
		outStackFrame->filename = sf.filename.c_str();
		outStackFrame->line = sf.line;
		outStackFrame->code = sf.code.c_str();
		outStackFrame->function = sf.function.c_str();

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_MakeArray(Joint_Type elementType, Joint_SizeT size, Joint_ArrayHandle* outHandle)
	{
		JOINT_CPP_WRAP_BEGIN
		*outHandle = new Joint_Array(elementType, size);
		JOINT_CPP_WRAP_END
	}


	void Joint_IncRefArray(Joint_ArrayHandle handle)
	{
		if (handle == JOINT_NULL_HANDLE)
			return;

		if (++handle->refCount <= 1)
			JOINT_TERMINATE("Inconsistent reference counter!");
	}


	void Joint_DecRefArray(Joint_ArrayHandle handle)
	{
		if (handle == JOINT_NULL_HANDLE)
			return;

		auto refs = --handle->refCount;
		if (refs < 0)
			JOINT_TERMINATE("Inconsistent reference counter!");

		if (refs == 0)
		{
			JOINT_CPP_WRAP_BEGIN
			delete handle;
			JOINT_CPP_WRAP_END_VOID
		}
	}


	Joint_Error Joint_ArrayGetSize(Joint_ArrayHandle handle, Joint_SizeT* outSize)
	{
		JOINT_CPP_WRAP_BEGIN
		JOINT_CHECK(handle != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outSize, JOINT_ERROR_INVALID_PARAMETER);
		*outSize = handle->elements.size();
		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_ArraySet(Joint_ArrayHandle handle, Joint_SizeT index, Joint_Value value)
	{
		JOINT_CPP_WRAP_BEGIN
		JOINT_CHECK(handle != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(index < handle->elements.size(), JOINT_ERROR_INDEX_OUT_OF_RANGE);
		handle->Set(index, value);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_ArrayGet(Joint_ArrayHandle handle, Joint_SizeT index, Joint_Value* outValue)
	{
		JOINT_CPP_WRAP_BEGIN
		JOINT_CHECK(handle != JOINT_NULL_HANDLE, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(index < handle->elements.size(), JOINT_ERROR_INDEX_OUT_OF_RANGE);
		*outValue = handle->Get(index);
		JOINT_CPP_WRAP_END
	}


#ifdef JOINT_PLATFORM_POSIX
	const char* JointAux_GetModuleName(Joint_FunctionPtr symbol)
	{
		Dl_info dl_info;
		dladdr((void*)symbol, &dl_info);
		return dl_info.dli_fname;
	}
#else
	const char* JointAux_GetModuleName(Joint_FunctionPtr symbol)
	{ return "<Unknown module>"; }
#endif

}
