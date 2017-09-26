#include <joint/devkit/CppWrappers.hpp>
#include <joint/devkit/JointException.hpp>
#include <joint/devkit/Logger.hpp>
#include <joint/devkit/MakeUnique.hpp>
#include <joint/private/JointStructs.h>
#include <joint/private/JsonParser.hpp>
#include <joint/public/JointConfig.h>

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


using namespace joint;
using namespace joint::devkit;


static const char* LoggerName = "Joint.Core";
JOINT_DEVKIT_LOGGER(LoggerName)

static void DefaultLogCallback(JointCore_LogLevel logLevel, const char* subsystem, const char* message)
{
	const char* alignment = "        ";
	const char* log_level_str = JointCore_LogLevelToString(logLevel);
#if JOINT_PLATFORM_POSIX
	if (isatty(2))
	{
		const char* subsystem_ns_color = "\33[0;32m";
		const char* subsystem_name_color = "\33[1;32m";
		const char* log_color = "\33[0m";

		switch (logLevel)
		{
		case JOINT_CORE_LOGLEVEL_DEBUG:    log_color = "\33[2;37m";  break;
		case JOINT_CORE_LOGLEVEL_INFO:     log_color = "\33[0m";     break;
		case JOINT_CORE_LOGLEVEL_WARNING:  log_color = "\33[1;33m";  break;
		case JOINT_CORE_LOGLEVEL_ERROR:    log_color = "\33[1;31m";  break;
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
		fprintf(stderr, "[%s]%.*s[%s] %s\n", JointCore_LogLevelToString(logLevel), (int)(sizeof("Warning") - strlen(log_level_str)), alignment, subsystem, message);
}


static std::mutex                     g_mutex;
static JointCore_LogCallback_Func*        g_logCallback = &DefaultLogCallback;
//static std::atomic<JointCore_LogLevel>    g_logLevel(JOINT_CORE_LOGLEVEL_DEBUG);
static std::atomic<JointCore_LogLevel>    g_logLevel(JOINT_CORE_LOGLEVEL_WARNING);

extern "C"
{

	const char* JointCore_LogLevelToString(JointCore_LogLevel logLevel)
	{
		switch (logLevel)
		{
		case JOINT_CORE_LOGLEVEL_DEBUG:    return "Debug";
		case JOINT_CORE_LOGLEVEL_INFO:     return "Info";
		case JOINT_CORE_LOGLEVEL_WARNING:  return "Warning";
		case JOINT_CORE_LOGLEVEL_ERROR:    return "Error";
		default:                      return "Unknown log level";
		}
	}

	JointCore_Error Joint_SetLogCallback(JointCore_LogCallback_Func* logCallback)
	{
		JOINT_CPP_WRAP_BEGIN
		std::lock_guard<std::mutex> l(g_mutex);
		g_logCallback = (logCallback == nullptr) ? &DefaultLogCallback : logCallback;
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_SetLogLevel(JointCore_LogLevel logLevel)
	{
		JOINT_CPP_WRAP_BEGIN
		JOINT_CHECK(logLevel >= JOINT_CORE_LOGLEVEL_DEBUG && logLevel <= JOINT_CORE_LOGLEVEL_ERROR, JOINT_CORE_ERROR_INVALID_PARAMETER);
		g_logLevel = logLevel;
		JOINT_CPP_WRAP_END
	}


	JointCore_LogLevel Joint_GetLogLevel()
	{ return g_logLevel.load(std::memory_order_relaxed); }


	void Joint_Log(JointCore_LogLevel logLevel, const char* subsystem, const char* format, ...)
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


	const char* JointCore_ErrorToString(JointCore_Error err)
	{
#define DETAIL_JOINT_ERR_TO_STRING(Val_) case Val_: return #Val_
		switch (err)
		{
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_NONE);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_CAST_FAILED);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_EXCEPTION);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_GENERIC);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_NO_SUCH_BINDING);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_NO_SUCH_MODULE);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_NOT_IMPLEMENTED);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_INVALID_PARAMETER);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_OUT_OF_MEMORY);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_IMPLEMENTATION_ERROR);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_INVALID_INTERFACE_CHECKSUM);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_INDEX_OUT_OF_RANGE);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_IO_ERROR);
		DETAIL_JOINT_ERR_TO_STRING(JOINT_CORE_ERROR_INVALID_MANIFEST);
		}
#undef DETAIL_JOINT_ERR_TO_STRING

		return "Unknown error";
	}


	JointCore_Error Joint_ReadManifestFromFile(const char* path, JointCore_ManifestHandle* outManifest)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(outManifest, JOINT_CORE_ERROR_INVALID_PARAMETER);

		GetLogger().Info() << "ReadManifestFromFile(path: " << (path ? path : "null") << ")";
		JsonNode n = JsonParser::Parse(path);

		std::string path_str(path);
		size_t last_slash = path_str.find_last_of("/\\");

		std::string location = (last_slash == std::string::npos) ? "." : path_str.substr(0, last_slash);

		*outManifest = new Joint_Manifest(std::move(n), std::move(location));
		GetLogger().Debug() << "Manifest: " << *outManifest;

		JOINT_CPP_WRAP_END
	}


	void Joint_DeleteManifest(JointCore_ManifestHandle handle)
	{
		JOINT_CPP_WRAP_BEGIN

		if (handle)
			delete handle;

		JOINT_CPP_WRAP_END_VOID
	}


	JointCore_Error Joint_GetManifestLocation(JointCore_ManifestHandle manifest, const char** outLocation)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(manifest, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outLocation, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outLocation = manifest->location.c_str();

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestRootNode(JointCore_ManifestHandle manifest, JointCore_ManifestNodeHandle* outNode)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(manifest, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outNode, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outNode = &manifest->rootNode;

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeType(JointCore_ManifestNodeHandle node, JointCore_ManifestNodeType* outType)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outType, JOINT_CORE_ERROR_INVALID_PARAMETER);

		switch (node->GetType())
		{
		case JsonNode::Type::Null:    *outType = JOINT_CORE_MANIFEST_NODE_NULL; break;
		case JsonNode::Type::Boolean: *outType = JOINT_CORE_MANIFEST_NODE_BOOLEAN; break;
		case JsonNode::Type::Integer: *outType = JOINT_CORE_MANIFEST_NODE_INTEGER; break;
		case JsonNode::Type::Float:   *outType = JOINT_CORE_MANIFEST_NODE_FLOAT; break;
		case JsonNode::Type::String:  *outType = JOINT_CORE_MANIFEST_NODE_STRING; break;
		case JsonNode::Type::Array:   *outType = JOINT_CORE_MANIFEST_NODE_ARRAY; break;
		case JsonNode::Type::Object:  *outType = JOINT_CORE_MANIFEST_NODE_OBJECT; break;
		default: JOINT_THROW(JOINT_CORE_ERROR_INVALID_PARAMETER);
		}

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeBooleanValue(JointCore_ManifestNodeHandle node, JointCore_Bool* outVal)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outVal, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outVal = (JointCore_Bool)node->AsBoolean();

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeIntegerValue(JointCore_ManifestNodeHandle node, int64_t* outVal)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outVal, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outVal = node->AsInteger();

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeFloatValue(JointCore_ManifestNodeHandle node, double* outVal)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outVal, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outVal = node->AsFloat();

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeStringValue(JointCore_ManifestNodeHandle node, const char** outVal)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outVal, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outVal = node->AsString().c_str();

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeChildrenCount(JointCore_ManifestNodeHandle node, JointCore_SizeT* outCount)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outCount, JOINT_CORE_ERROR_INVALID_PARAMETER);

		switch (node->GetType())
		{
		case JsonNode::Type::Array:  *outCount = node->AsArray().size(); break;
		case JsonNode::Type::Object: *outCount = node->AsObject().size(); break;
		default: JOINT_THROW(JOINT_CORE_ERROR_INVALID_PARAMETER);
		}

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeArrayElement(JointCore_ManifestNodeHandle node, JointCore_SizeT index, JointCore_ManifestNodeHandle* outNode)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outNode, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outNode = static_cast<JointCore_ManifestNodeHandle>(&node->AsArray().at(index));

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeObjectElementByKey(JointCore_ManifestNodeHandle node, const char* key, JointCore_ManifestNodeHandle* outValue)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outValue, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outValue = static_cast<JointCore_ManifestNodeHandle>(const_cast<JsonNode*>(&node->AsObject().at(key)));

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_MakeException(const char* message, const JointCore_StackFrame* backtrace, JointCore_SizeT backtraceSize, JointCore_ExceptionHandle* outException)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(outException, JOINT_CORE_ERROR_INVALID_PARAMETER);

		std::vector<JointCore_StackFrameData> bt;
		bt.reserve(backtraceSize);
		for (JointCore_SizeT i = 0; i < backtraceSize; ++i)
		{
			auto sf = backtrace[i];
			bt.push_back(JointCore_StackFrameData{sf.module, sf.filename, sf.line, sf.code, sf.function});
		}
		*outException = new Joint_Exception{message, bt};

		JOINT_CPP_WRAP_END
	}


	void Joint_ReleaseException(JointCore_ExceptionHandle handle)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);

		delete handle;

		JOINT_CPP_WRAP_END_VOID
	}


	JointCore_Error Joint_GetExceptionMessageSize(JointCore_ExceptionHandle handle, JointCore_SizeT* outSize)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outSize, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outSize = handle->message.size() + 1;

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetExceptionMessage(JointCore_ExceptionHandle handle, char* buf, JointCore_SizeT bufSize)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(buf, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(bufSize >= handle->message.size() + 1, JOINT_CORE_ERROR_INVALID_PARAMETER);

		strcpy(buf, handle->message.c_str());

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetExceptionBacktraceSize(JointCore_ExceptionHandle handle, JointCore_SizeT* outSize)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outSize, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outSize = handle->backtrace.size();

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetExceptionBacktraceEntry(JointCore_ExceptionHandle handle, JointCore_SizeT index, JointCore_StackFrame* outStackFrame)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(index < handle->backtrace.size(), JOINT_CORE_ERROR_INVALID_PARAMETER);

		const auto& sf = handle->backtrace[index];
		outStackFrame->module = sf.module.c_str();
		outStackFrame->filename = sf.filename.c_str();
		outStackFrame->line = sf.line;
		outStackFrame->code = sf.code.c_str();
		outStackFrame->function = sf.function.c_str();

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_MakeArray(JointCore_Type elementType, JointCore_SizeT size, JointCore_ArrayHandle* outArray)
	{
		JOINT_CPP_WRAP_BEGIN
		*outArray = new Joint_Array(elementType, size);
		JOINT_CPP_WRAP_END
	}


	void Joint_IncRefArray(JointCore_ArrayHandle handle)
	{
		if (handle == JOINT_CORE_NULL_HANDLE)
			return;

		if (++handle->refCount <= 1)
			JOINT_TERMINATE("Inconsistent reference counter!");
	}


	void Joint_DecRefArray(JointCore_ArrayHandle handle)
	{
		if (handle == JOINT_CORE_NULL_HANDLE)
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


	JointCore_Error Joint_ArrayGetSize(JointCore_ArrayHandle handle, JointCore_SizeT* outSize)
	{
		JOINT_CPP_WRAP_BEGIN
		JOINT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outSize, JOINT_CORE_ERROR_INVALID_PARAMETER);
		*outSize = handle->elements.size();
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_ArraySet(JointCore_ArrayHandle handle, JointCore_SizeT index, JointCore_Value value)
	{
		JOINT_CPP_WRAP_BEGIN
		JOINT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(index < handle->elements.size(), JOINT_CORE_ERROR_INDEX_OUT_OF_RANGE);
		handle->Set(index, value);
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_ArrayGet(JointCore_ArrayHandle handle, JointCore_SizeT index, JointCore_Value* outValue)
	{
		JOINT_CPP_WRAP_BEGIN
		JOINT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(index < handle->elements.size(), JOINT_CORE_ERROR_INDEX_OUT_OF_RANGE);
		*outValue = handle->Get(index);
		JOINT_CPP_WRAP_END
	}


#ifdef JOINT_PLATFORM_POSIX
	const char* JointAux_GetModuleName(JointCore_FunctionPtr symbol)
	{
		Dl_info dl_info;
		dladdr((void*)symbol, &dl_info);
		return dl_info.dli_fname;
	}
#else
	const char* JointAux_GetModuleName(JointCore_FunctionPtr symbol)
	{ return "<Unknown module>"; }
#endif


	void JointCore_FatalImpl(const char* subsystem, const char* file, int line, const char* func, const char* format, ...)
	{
		char message[1024];
		va_list argptr;
		va_start(argptr, format);
		vsnprintf(message, sizeof(message), format, argptr);
		va_end(argptr);

		Joint_Log(JOINT_CORE_LOGLEVEL_ERROR, subsystem, "Fatal condition at %s:%d in function %s: %s", file, line, func, message);

		std::terminate();
	}

}
