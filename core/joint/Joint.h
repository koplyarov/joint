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
		JOINT_ERROR_NOT_IMPLEMENTED = 1,
		JOINT_ERROR_INVALID_PARAMETER = 2,
		JOINT_ERROR_OUT_OF_MEMORY = 3,
		JOINT_ERROR_GENERIC = 4
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


	enum Joint_ParameterType
	{
		JOINT_PARAMETER_TYPE_BOOL    = 0,

		JOINT_PARAMETER_TYPE_I8      = 1,
		JOINT_PARAMETER_TYPE_U8      = 2,
		JOINT_PARAMETER_TYPE_I16     = 3,
		JOINT_PARAMETER_TYPE_U16     = 4,
		JOINT_PARAMETER_TYPE_I32     = 5,
		JOINT_PARAMETER_TYPE_U32     = 6,
		JOINT_PARAMETER_TYPE_I64     = 7,
		JOINT_PARAMETER_TYPE_U64     = 8,

		JOINT_PARAMETER_TYPE_F32     = 9,
		JOINT_PARAMETER_TYPE_F64     = 10,

		JOINT_PARAMETER_TYPE_UTF8    = 11,

		JOINT_PARAMETER_TYPE_OBJ     = 12,
	};


	struct Joint_Parameter
	{
		union
		{
			bool            b;

			int8_t          i8;
			uint8_t         u8;
			int16_t         i16;
			uint16_t        u16;
			int32_t         i32;
			uint32_t        u32;
			int64_t         i64;
			uint64_t        u64;

			float           f32;
			double          f64;

			const char*     utf8;

			void*           obj;
		} variant;

		Joint_ParameterType type;
	};


	typedef Joint_Error Joint_GetRootObject_Func(void* bindingUserData, Joint_ModuleHandleInternal module, const char* getterName, Joint_ObjectHandleInternal* outObject);
	typedef Joint_Error Joint_InvokeMethod_Func(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, int methodId, const Joint_Parameter* params, Joint_SizeT paramsCount);

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
	Joint_Error Joint_InvokeMethod(Joint_ObjectHandle obj, int methodId, const Joint_Parameter* params, Joint_SizeT paramsCount);
}

#endif
