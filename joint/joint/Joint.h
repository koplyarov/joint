#ifndef JOINT_JOINTCORE_H
#define JOINT_JOINTCORE_H


#include <stdint.h>


#ifdef _MSC_VER
#	ifdef joint_EXPORTS
#		define JOINT_API __declspec(dllexport)
#	else
#		define JOINT_API __declspec(dllimport)
#	endif
#else
#	define JOINT_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define JOINT_NULL_HANDLE NULL

	typedef const char* Joint_InterfaceId;

	struct Joint_Binding;
	typedef struct Joint_Binding* Joint_BindingHandle;

	struct Joint_Module;
	typedef struct Joint_Module* Joint_ModuleHandle;

	struct Joint_Object;
	typedef struct Joint_Object* Joint_ObjectHandle;

	struct Joint_Exception;
	typedef struct Joint_Exception* Joint_ExceptionHandle;


	typedef uint32_t Joint_SizeT;

	typedef void* Joint_ObjectHandleInternal;
	typedef void* Joint_ModuleHandleInternal;


	typedef enum
	{
		JOINT_ERROR_NONE = 0,
		JOINT_ERROR_GENERIC = 1,
		JOINT_ERROR_NO_SUCH_BINDING = 2,
		JOINT_ERROR_NO_SUCH_MODULE = 3,
		JOINT_ERROR_NOT_IMPLEMENTED = 4,
		JOINT_ERROR_INVALID_PARAMETER = 5,
		JOINT_ERROR_OUT_OF_MEMORY = 6,
		JOINT_ERROR_IMPLEMENTATION_ERROR = 7,
		JOINT_ERROR_UNEXPECTED_TYPE = 8
	} Joint_Error;

	JOINT_API const char* Joint_ErrorToString(Joint_Error err);


	typedef enum
	{
		JOINT_LOGLEVEL_DEBUG    = 0,
		JOINT_LOGLEVEL_INFO     = 1,
		JOINT_LOGLEVEL_WARNING  = 2,
		JOINT_LOGLEVEL_ERROR    = 3
	} Joint_LogLevel;

	JOINT_API const char* Joint_LogLevelToString(Joint_LogLevel logLevel);

	typedef void Joint_LogCallback_Func(Joint_LogLevel logLevel, const char* category, const char* message);
	JOINT_API Joint_Error Joint_SetLogCallback(Joint_LogCallback_Func* logCallback);
	JOINT_API Joint_Error Joint_SetLogLevel(Joint_LogLevel logLevel);

	JOINT_API Joint_LogLevel Joint_GetLogLevel();
	JOINT_API void Joint_Log(Joint_LogLevel logLevel, const char* subsystem, const char* format, ...);


	typedef enum
	{
		JOINT_TYPE_VOID       = 1,

		JOINT_TYPE_BOOL       = 2,

		JOINT_TYPE_I8         = 3,
		JOINT_TYPE_U8         = 4,
		JOINT_TYPE_I16        = 5,
		JOINT_TYPE_U16        = 6,
		JOINT_TYPE_I32        = 7,
		JOINT_TYPE_U32        = 8,
		JOINT_TYPE_I64        = 9,
		JOINT_TYPE_U64        = 10,

		JOINT_TYPE_F32        = 11,
		JOINT_TYPE_F64        = 12,

		JOINT_TYPE_UTF8       = 13,

		JOINT_TYPE_OBJ        = 14,

		JOINT_TYPE_EXCEPTION  = 15
	} Joint_Type;


	typedef struct
	{
		union
		{
			int                     b;

			int8_t                  i8;
			uint8_t                 u8;
			int16_t                 i16;
			uint16_t                u16;
			int32_t                 i32;
			uint32_t                u32;
			int64_t                 i64;
			uint64_t                u64;

			float                   f32;
			double                  f64;

			const char*             utf8;

			Joint_ObjectHandle      obj;
			Joint_ExceptionHandle   ex;
		} value;

		Joint_Type type;
	} Joint_Variant;


	typedef Joint_Error Joint_ReleaseRetValue_Func(Joint_Variant value);

	typedef struct
	{
		Joint_Variant                   variant;
		Joint_ReleaseRetValue_Func*     releaseValue;
	} Joint_RetValue;

	struct Joint_Context;
	typedef struct Joint_Context* Joint_ContextHandle;

	typedef Joint_Error Joint_GetRootObject_Func(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, const char* getterName, Joint_ObjectHandle* outObject);
	typedef Joint_Error Joint_ReleaseObject_Func(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal object);
	typedef Joint_Error Joint_InvokeMethod_Func(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue);
	typedef Joint_Error Joint_CastObject_Func(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, Joint_InterfaceId interfaceId, Joint_ObjectHandleInternal* outRetValue);
	typedef Joint_Error Joint_LoadModule_Func(void* bindingUserData, const char* moduleName, Joint_ModuleHandleInternal* outModule);
	typedef Joint_Error Joint_UnloadModule_Func(void* bindingUserData, Joint_ModuleHandleInternal module);
	typedef Joint_Error Joint_DeinitBinding_Func(void* bindingUserData);

	typedef struct
	{
		Joint_InvokeMethod_Func*    invokeMethod;
		Joint_ReleaseObject_Func*   releaseObject;
		Joint_CastObject_Func*      castObject;
		Joint_GetRootObject_Func*   getRootObject;
		Joint_LoadModule_Func*      loadModule;
		Joint_UnloadModule_Func*    unloadModule;
		Joint_DeinitBinding_Func*   deinitBinding;

		const char*                 name;
	} Joint_BindingDesc;

	JOINT_API Joint_Error Joint_MakeContext(Joint_ContextHandle *outJointCtx);
	JOINT_API Joint_Error Joint_ReleaseContext(Joint_ContextHandle jointCtx);

	JOINT_API Joint_Error Joint_MakeBinding(Joint_BindingDesc desc, void* userData, Joint_BindingHandle* outBinding);
	JOINT_API Joint_Error Joint_IncRefBinding(Joint_BindingHandle handle);
	JOINT_API Joint_Error Joint_DecRefBinding(Joint_BindingHandle handle);

	JOINT_API Joint_Error Joint_LoadModule(Joint_BindingHandle binding, const char* moduleName, Joint_ModuleHandle* outModule);
	JOINT_API Joint_Error Joint_MakeModule(Joint_BindingHandle binding, Joint_ModuleHandleInternal internal, Joint_ModuleHandle* outModule);
	JOINT_API Joint_Error Joint_IncRefModule(Joint_ModuleHandle handle);
	JOINT_API Joint_Error Joint_DecRefModule(Joint_ModuleHandle handle);

	JOINT_API Joint_Error Joint_CreateObject(Joint_ModuleHandle module, Joint_ObjectHandleInternal internal, Joint_ObjectHandle* outObject);

	JOINT_API Joint_Error Joint_GetRootObject(Joint_ModuleHandle module, const char* getterName, Joint_ObjectHandle* outObject);
	JOINT_API Joint_Error Joint_InvokeMethod(Joint_ObjectHandle obj, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue);
	JOINT_API Joint_Error Joint_ObtainRetValue(Joint_RetValue value, Joint_Variant* outValue);
	JOINT_API Joint_Error Joint_ReleaseRetValue(Joint_RetValue value);
	JOINT_API Joint_Error Joint_IncRefObject(Joint_ObjectHandle handle);
	JOINT_API Joint_Error Joint_DecRefObject(Joint_ObjectHandle handle);
	JOINT_API Joint_Error Joint_CastObject(Joint_ObjectHandle handle, Joint_InterfaceId interfaceId, Joint_ObjectHandle* outHandle);

	JOINT_API Joint_Error Joint_MakeException(const char* message, Joint_ExceptionHandle* outHandle);
	JOINT_API Joint_Error Joint_ReleaseException(Joint_ExceptionHandle handle);
	JOINT_API Joint_Error Joint_GetExceptionMessageSize(Joint_ExceptionHandle handle, Joint_SizeT* outSize);
	JOINT_API Joint_Error Joint_GetExceptionMessage(Joint_ExceptionHandle handle, char* buf, Joint_SizeT bufSize);
	JOINT_API Joint_Error Joint_GetExceptionBacktraceSize(Joint_ExceptionHandle handle, Joint_SizeT* outSize);
	JOINT_API Joint_Error Joint_GetExceptionBacktraceEntrySize(Joint_ExceptionHandle handle, Joint_SizeT index, Joint_SizeT* outSize);
	JOINT_API Joint_Error Joint_GetExceptionBacktraceEntry(Joint_ExceptionHandle handle, Joint_SizeT index, char* buf, Joint_SizeT bufSize);

#ifdef __cplusplus
}
#endif

#endif
