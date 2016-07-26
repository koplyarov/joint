#ifndef BINDINGS_CPP_CPPBINDING_H
#define BINDINGS_CPP_CPPBINDING_H


#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>


namespace joint_cpp {
namespace binding
{

	class Binding
	{
		JOINT_DEVKIT_LOGGER("Joint.C++.Binding")

	public:
		Binding();
		~Binding();

		static Joint_Error Deinit(void* bindingUserData);
		static Joint_Error LoadModule(void* bindingUserData, const char* moduleName, Joint_ModuleHandleInternal* outModule);
		static Joint_Error UnloadModule(void* bindingUserData, Joint_ModuleHandleInternal module);
		static Joint_Error GetRootObject(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, const char* getterName, Joint_ObjectHandle* outObject);
		static Joint_Error InvokeMethod(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue);
		static Joint_Error ReleaseObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj);
		static Joint_Error CastObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, Joint_InterfaceId interfaceId, Joint_ObjectHandleInternal* outRetValue);
		static Joint_Error ReleaseRetValue(Joint_Variant value);
	};

}}

#endif
