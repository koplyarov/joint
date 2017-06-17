#ifndef JOINT_JOINTCORE_H
#define JOINT_JOINTCORE_H


#include <joint/JointConfig.h>

#include <stdint.h>


#ifdef joint_EXPORTS
#	define JOINT_CORE_API JOINT_CORE_EXPORT
#else
#	define JOINT_CORE_API JOINT_CORE_IMPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define JOINT_CORE_NULL_HANDLE NULL

#define JOINT_CORE_TRUE 1
#define JOINT_CORE_FALSE 0

	typedef int32_t JointCore_Bool;

	typedef uint32_t JointCore_InterfaceChecksum;
	typedef const char* JointCore_InterfaceId;

	struct Joint_ManifestNode;
	typedef const struct Joint_ManifestNode* JointCore_ManifestNodeHandle;

	struct Joint_Manifest;
	typedef struct Joint_Manifest* JointCore_ManifestHandle;

	struct Joint_Exception;
	typedef struct Joint_Exception* JointCore_ExceptionHandle;

	struct Joint_Array;
	typedef struct Joint_Array* JointCore_ArrayHandle;


	typedef uint32_t JointCore_SizeT;


	typedef enum
	{
		JOINT_CORE_ERROR_NONE = 0,
		JOINT_CORE_ERROR_CAST_FAILED = 1,
		JOINT_CORE_ERROR_EXCEPTION = 2,
		JOINT_CORE_ERROR_GENERIC = 3,
		JOINT_CORE_ERROR_NO_SUCH_BINDING = 4,
		JOINT_CORE_ERROR_NO_SUCH_MODULE = 5,
		JOINT_CORE_ERROR_NOT_IMPLEMENTED = 6,
		JOINT_CORE_ERROR_INVALID_PARAMETER = 7,
		JOINT_CORE_ERROR_OUT_OF_MEMORY = 8,
		JOINT_CORE_ERROR_IMPLEMENTATION_ERROR = 9,
		JOINT_CORE_ERROR_INVALID_INTERFACE_CHECKSUM = 10,
		JOINT_CORE_ERROR_INDEX_OUT_OF_RANGE = 11,
		JOINT_CORE_ERROR_IO_ERROR = 12,
		JOINT_CORE_ERROR_INVALID_MANIFEST = 13
	} JointCore_Error;

	JOINT_CORE_API const char* JointCore_ErrorToString(JointCore_Error err);


	typedef enum
	{
		JOINT_CORE_LOGLEVEL_DEBUG    = 0,
		JOINT_CORE_LOGLEVEL_INFO     = 1,
		JOINT_CORE_LOGLEVEL_WARNING  = 2,
		JOINT_CORE_LOGLEVEL_ERROR    = 3
	} JointCore_LogLevel;

	JOINT_CORE_API const char* JointCore_LogLevelToString(JointCore_LogLevel logLevel);

	typedef void JointCore_LogCallback_Func(JointCore_LogLevel logLevel, const char* category, const char* message);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_SetLogCallback(JointCore_LogCallback_Func* logCallback);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_SetLogLevel(JointCore_LogLevel logLevel);

	JOINT_CORE_API JointCore_LogLevel Joint_GetLogLevel();
	JOINT_CORE_API void Joint_Log(JointCore_LogLevel logLevel, const char* subsystem, const char* format, ...);


	typedef enum
	{
		JOINT_CORE_MANIFEST_NODE_NULL,
		JOINT_CORE_MANIFEST_NODE_BOOLEAN,
		JOINT_CORE_MANIFEST_NODE_INTEGER,
		JOINT_CORE_MANIFEST_NODE_FLOAT,
		JOINT_CORE_MANIFEST_NODE_STRING,
		JOINT_CORE_MANIFEST_NODE_ARRAY,
		JOINT_CORE_MANIFEST_NODE_OBJECT
	} JointCore_ManifestNodeType;


	typedef enum
	{
		JOINT_CORE_TYPE_VOID       = 1,
		JOINT_CORE_TYPE_BOOL       = 2,
		JOINT_CORE_TYPE_I8         = 3,
		JOINT_CORE_TYPE_U8         = 4,
		JOINT_CORE_TYPE_I16        = 5,
		JOINT_CORE_TYPE_U16        = 6,
		JOINT_CORE_TYPE_I32        = 7,
		JOINT_CORE_TYPE_U32        = 8,
		JOINT_CORE_TYPE_I64        = 9,
		JOINT_CORE_TYPE_U64        = 10,
		JOINT_CORE_TYPE_F32        = 11,
		JOINT_CORE_TYPE_F64        = 12,
		JOINT_CORE_TYPE_UTF8       = 13,
		JOINT_CORE_TYPE_ENUM       = 14,
		JOINT_CORE_TYPE_STRUCT     = 15,
		JOINT_CORE_TYPE_OBJ        = 16,
		JOINT_CORE_TYPE_ARRAY      = 17
	} JointCore_TypeId;


	typedef struct
	{
		int32_t                     membersCount;
		struct _JointCore_Type_t*   memberTypes;
	} JointCore_StructDescriptor;

	typedef union _JointCore_TypePayload_t
	{
		struct _JointCore_Type_t*   arrayElementType;
		JointCore_InterfaceChecksum interfaceChecksum;
		JointCore_StructDescriptor* structDescriptor;
	} JointCore_TypePayload;


	typedef struct _JointCore_Type_t
	{
		JointCore_TypeId            id;
		JointCore_TypePayload       payload;
	} JointCore_Type;

	////////////////////////////////////////////////////////////////////////////////

	typedef struct JointCore_Parameter_s JointCore_Parameter;
	typedef struct JointCore_RetValue_s JointCore_RetValue;

	////////////////////////////////////////////////////////////////////////////////


	typedef struct JointCore_ObjectAccessor_s JointCore_ObjectAccessor;

	typedef struct
	{
		void (*AddRef)(void* component);
		void (*Release)(void* component);
		JointCore_Error (*CastObject)(void* component, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectAccessor* outAccessor);
		JointCore_Error (*InvokeMethod)(void* component, JointCore_SizeT methodId, const JointCore_Parameter* params, JointCore_SizeT paramsCount, JointCore_Type retType, JointCore_RetValue* outRetValue);
	} JointCore_ObjectAccessorVTable;


	struct JointCore_ObjectAccessor_s
	{
		const JointCore_ObjectAccessorVTable*   VTable;
		void*                                   Instance;
	};

#define JOINT_CORE_INCREF_ACCESSOR(Accessor_) \
		do { \
			if ((Accessor_).Instance) \
				(Accessor_).VTable->AddRef((Accessor_).Instance); \
		} while (0)

#define JOINT_CORE_DECREF_ACCESSOR(Accessor_) \
		do { \
			if ((Accessor_).Instance) \
				(Accessor_).VTable->Release((Accessor_).Instance); \
		} while (0)

#define JOINT_CORE_IS_NULL(Accessor_) \
		(!(Accessor_).Instance)

#define JOINT_CORE_INIT_ACCESSOR(Accessor_) \
		do { \
			(Accessor_).Instance = NULL; \
		} while(0)

	////////////////////////////////////////////////////////////////////////////////

	typedef struct JointCore_ModuleAccessor_s JointCore_ModuleAccessor;

	typedef struct
	{
		void (*AddRef)(void* module);
		void (*Release)(void* module);
		JointCore_Error (*GetRootObject)(void* module, const char* getterName, JointCore_ObjectAccessor* outObject);
	} JointCore_ModuleAccessorVTable;


	struct JointCore_ModuleAccessor_s
	{
		const JointCore_ModuleAccessorVTable*   VTable;
		void*                                   Instance;
	};

	////////////////////////////////////////////////////////////////////////////////

	typedef struct JointCore_BindingAccessor_s JointCore_BindingAccessor;

	typedef struct
	{
		void (*AddRef)(void* binding);
		void (*Release)(void* binding);
		JointCore_Error (*LoadModule)(void* binding, JointCore_ManifestHandle moduleManifest, JointCore_ModuleAccessor* outModule);
	} JointCore_BindingAccessorVTable;


	struct JointCore_BindingAccessor_s
	{
		const JointCore_BindingAccessorVTable*   VTable;
		void*                                    Instance;
	};

	////////////////////////////////////////////////////////////////////////////////

	typedef union _JointCore_Value_t
	{
		JointCore_Bool              b;
		int8_t                      i8;
		uint8_t                     u8;
		int16_t                     i16;
		uint16_t                    u16;
		int32_t                     i32;
		uint32_t                    u32;
		int64_t                     i64;
		uint64_t                    u64;
		float                       f32;
		double                      f64;
		const char*                 utf8;
		int32_t                     e;
		union _JointCore_Value_t*   members;
		JointCore_ObjectAccessor    obj;
		JointCore_ArrayHandle       array;
	} JointCore_Value;


	struct JointCore_Parameter_s
	{
		JointCore_Value value;
		JointCore_Type  type;
	};


	typedef JointCore_Error JointCore_ReleaseRetValue_Func(JointCore_Type type, JointCore_Value value);

	struct JointCore_RetValue_s
	{
		union
		{
			JointCore_Value               value;
			JointCore_ExceptionHandle     ex;
		} result;
		JointCore_ReleaseRetValue_Func*   releaseValue;
	};

	struct Joint_Context;
	typedef struct Joint_Context* JointCore_ContextHandle;

	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_MakeContext(JointCore_ContextHandle *outJointCtx);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_ReleaseContext(JointCore_ContextHandle jointCtx);

	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_ReadManifestFromFile(const char* path, JointCore_ManifestHandle* outManifest);
	JOINT_CORE_API void Joint_DeleteManifest(JointCore_ManifestHandle handle);

	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestLocation(JointCore_ManifestHandle manifest, const char** outLocation);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestRootNode(JointCore_ManifestHandle manifest, JointCore_ManifestNodeHandle* outNode);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeType(JointCore_ManifestNodeHandle node, JointCore_ManifestNodeType* outType);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeBooleanValue(JointCore_ManifestNodeHandle node, JointCore_Bool* outVal);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeIntegerValue(JointCore_ManifestNodeHandle node, int64_t* outVal);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeFloatValue(JointCore_ManifestNodeHandle node, double* outVal);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeStringValue(JointCore_ManifestNodeHandle node, const char** outVal);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeChildrenCount(JointCore_ManifestNodeHandle node, JointCore_SizeT* outCount);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeArrayElement(JointCore_ManifestNodeHandle node, JointCore_SizeT index, JointCore_ManifestNodeHandle* outNode);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeObjectElementByKey(JointCore_ManifestNodeHandle node, const char* key, JointCore_ManifestNodeHandle* outValue);

	typedef struct
	{
		const char*      module;
		const char*      filename;
		JointCore_SizeT  line;
		const char*      code;
		const char*      function;
	} JointCore_StackFrame;

	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_MakeException(const char* message, const JointCore_StackFrame* backtrace, JointCore_SizeT backtraceSize, JointCore_ExceptionHandle* outException);
	JOINT_CORE_API void Joint_ReleaseException(JointCore_ExceptionHandle handle);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetExceptionMessageSize(JointCore_ExceptionHandle handle, JointCore_SizeT* outSize);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetExceptionMessage(JointCore_ExceptionHandle handle, char* buf, JointCore_SizeT bufSize);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetExceptionBacktraceSize(JointCore_ExceptionHandle handle, JointCore_SizeT* outSize);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetExceptionBacktraceEntry(JointCore_ExceptionHandle handle, JointCore_SizeT index, JointCore_StackFrame* outStackFrame);

	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_MakeArray(JointCore_Type elementType, JointCore_SizeT size, JointCore_ArrayHandle* outArray);
	JOINT_CORE_API void Joint_IncRefArray(JointCore_ArrayHandle handle);
	JOINT_CORE_API void Joint_DecRefArray(JointCore_ArrayHandle handle);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_ArrayGetSize(JointCore_ArrayHandle handle, JointCore_SizeT* outSize);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_ArraySet(JointCore_ArrayHandle handle, JointCore_SizeT index, JointCore_Value value);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_ArrayGet(JointCore_ArrayHandle handle, JointCore_SizeT index, JointCore_Value* outValue);

	typedef void (*JointCore_FunctionPtr)();
	JOINT_CORE_API const char* JointAux_GetModuleName(JointCore_FunctionPtr symbol);

#ifdef __cplusplus
}
#endif

#endif
