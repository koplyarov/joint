#ifndef JOINT_JOINT_H
#define JOINT_JOINT_H


#include <stdint.h>


extern "C"
{
#define JOINT_NULL_HANDLE nullptr

	struct Joint_Binding;
	typedef Joint_Binding* Joint_BindingHandle;

	struct Joint_Module;
	typedef Joint_Module* Joint_ModuleHandle;

	struct Joint_Object;
	typedef Joint_Object* Joint_ObjectHandle;


	typedef uint32_t Joint_SizeT;

	typedef void* Joint_ObjectHandleInternal;
	typedef void* Joint_ModuleHandleInternal;


	enum Joint_Error
	{
		JOINT_ERROR_NONE = 0,
		JOINT_ERROR_GENERIC = 1,
		JOINT_ERROR_NOT_IMPLEMENTED = 2,
		JOINT_ERROR_INVALID_PARAMETER = 3,
		JOINT_ERROR_OUT_OF_MEMORY = 4,
		JOINT_ERROR_IMPLEMENTATION_ERROR = 5,
		JOINT_ERROR_UNEXPECTED_TYPE = 6
	};

	const char* Joint_ErrorToString(Joint_Error err);


	enum Joint_LogLevel
	{
		JOINT_LOGLEVEL_DEBUG    = 0,
		JOINT_LOGLEVEL_INFO     = 1,
		JOINT_LOGLEVEL_WARNING  = 2,
		JOINT_LOGLEVEL_ERROR    = 3
	};

	typedef void Joint_LogCallback_Func(const char* message);
	Joint_Error Joint_SetLogCallback(Joint_LogCallback_Func* logCallback);
	Joint_Error Joint_SetLogLevel(Joint_LogLevel logLevel);

	void Joint_Log(Joint_LogLevel logLevel, const char* subsystem, const char* format, ...);


	enum Joint_Type
	{
		JOINT_TYPE_VOID    = 1,

		JOINT_TYPE_BOOL    = 2,

		JOINT_TYPE_I8      = 3,
		JOINT_TYPE_U8      = 4,
		JOINT_TYPE_I16     = 5,
		JOINT_TYPE_U16     = 6,
		JOINT_TYPE_I32     = 7,
		JOINT_TYPE_U32     = 8,
		JOINT_TYPE_I64     = 9,
		JOINT_TYPE_U64     = 10,

		JOINT_TYPE_F32     = 11,
		JOINT_TYPE_F64     = 12,

		JOINT_TYPE_UTF8    = 13,

		JOINT_TYPE_OBJ     = 14,
	};


	struct Joint_VariantInternal
	{
		union
		{
			bool                 b;

			int8_t               i8;
			uint8_t              u8;
			int16_t              i16;
			uint16_t             u16;
			int32_t              i32;
			uint32_t             u32;
			int64_t              i64;
			uint64_t             u64;

			float                f32;
			double               f64;

			const char*          utf8;

			Joint_ObjectHandleInternal   obj;
		} value;

		Joint_Type type;
	};


	struct Joint_Variant
	{
		union
		{
			bool                 b;

			int8_t               i8;
			uint8_t              u8;
			int16_t              i16;
			uint16_t             u16;
			int32_t              i32;
			uint32_t             u32;
			int64_t              i64;
			uint64_t             u64;

			float                f32;
			double               f64;

			const char*          utf8;

			Joint_ObjectHandle   obj;
		} value;

		Joint_Type type;
	};


	typedef Joint_Error Joint_ReleaseRetValue_Func(Joint_VariantInternal value);

	struct Joint_RetValueInternal
	{
		Joint_VariantInternal           variant;
		Joint_ReleaseRetValue_Func*     releaseValue;
	};

	struct Joint_RetValue
	{
		Joint_Variant                   variant;
		Joint_RetValueInternal			internal;
	};


	typedef Joint_Error Joint_GetRootObject_Func(void* bindingUserData, Joint_ModuleHandleInternal module, const char* getterName, Joint_ObjectHandleInternal* outObject);
	typedef Joint_Error Joint_InvokeMethod_Func(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValueInternal* outRetValue);
	typedef Joint_Error Joint_LoadModule_Func(void* bindingUserData, const char* moduleName, Joint_ModuleHandleInternal* outModule);
	typedef Joint_Error Joint_UnloadModule_Func(void* bindingUserData, Joint_ModuleHandleInternal module);
	typedef Joint_Error Joint_DeinitBinding_Func(void* bindingUserData);

	struct Joint_BindingDesc
	{
		Joint_GetRootObject_Func*   getRootObject;
		Joint_InvokeMethod_Func*    invokeMethod;
		Joint_LoadModule_Func*      loadModule;
		Joint_UnloadModule_Func*    unloadModule;
		Joint_DeinitBinding_Func*   deinitBinding;

		const char*                 name;
	};


	Joint_Error Joint_RegisterBinding(Joint_BindingDesc desc, void* userData, Joint_BindingHandle* outBinding);
	Joint_Error Joint_UnregisterBinding(Joint_BindingHandle handle);

	Joint_Error Joint_LoadModule(const char* bindingName, const char* moduleName, Joint_ModuleHandle* outModule);
	Joint_Error Joint_UnloadModule(Joint_ModuleHandle handle);

	Joint_Error Joint_GetRootObject(Joint_ModuleHandle module, const char* getterName, Joint_ObjectHandle* outObject);
	Joint_Error Joint_InvokeMethod(Joint_ObjectHandle obj, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue);
	Joint_Error Joint_ObtainRetValue(Joint_RetValue value, Joint_Variant* outValue);
	Joint_Error Joint_ReleaseRetValue(Joint_RetValue value);
}

#endif
