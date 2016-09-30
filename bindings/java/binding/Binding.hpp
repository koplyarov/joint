#ifndef BINDINGS_JAVA_JAVABINDING_H
#define BINDINGS_JAVA_JAVABINDING_H


#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>


namespace joint_java {
namespace binding
{

	class Binding
	{
		JOINT_DEVKIT_LOGGER("Joint.Java.Binding")

	public:
		Binding();
		~Binding();

		static Joint_Error Deinit(void* bindingUserData);
		static Joint_Error LoadModule(void* bindingUserData, const char* moduleName, Joint_ModuleHandleInternal* outModule);
		static Joint_Error UnloadModule(void* bindingUserData, Joint_ModuleHandleInternal module);
		static Joint_Error GetRootObject(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, const char* getterName, Joint_ObjectHandle* outObject);
		static Joint_Error InvokeMethod(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Parameter* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue);
		static Joint_Error ReleaseObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj);
		static Joint_Error CastObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, Joint_InterfaceId interfaceId, Joint_InterfaceChecksum checksum, Joint_ObjectHandleInternal* outRetValue);
	};

}}

#endif
