#ifndef BINDINGS_CPP_CPPBINDING_H
#define BINDINGS_CPP_CPPBINDING_H


#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>


namespace joint {
namespace cpp {
namespace binding
{

	class Binding
	{
		JOINT_DEVKIT_LOGGER("Joint.C++.Binding")

	public:
		Binding();
		~Binding();

		static JointCore_Error Deinit(void* bindingUserData);
		static JointCore_Error LoadModule(void* bindingUserData, JointCore_ManifestHandle moduleManifest, JointCore_ModuleHandleInternal* outModule);
		static JointCore_Error UnloadModule(void* bindingUserData, JointCore_ModuleHandleInternal module);
		static JointCore_Error GetRootObject(JointCore_ModuleHandle module, void* bindingUserData, JointCore_ModuleHandleInternal moduleInt, const char* getterName, JointCore_ObjectHandle* outObject);
		static JointCore_Error InvokeMethod(JointCore_ModuleHandle module, void* bindingUserData, JointCore_ModuleHandleInternal moduleInt, JointCore_ObjectHandleInternal obj, JointCore_SizeT methodId, const JointCore_Parameter* params, JointCore_SizeT paramsCount, JointCore_Type retType, JointCore_RetValue* outRetValue);
		static JointCore_Error ReleaseObject(void* bindingUserData, JointCore_ModuleHandleInternal module, JointCore_ObjectHandleInternal obj);
		static JointCore_Error CastObject(void* bindingUserData, JointCore_ModuleHandleInternal module, JointCore_ObjectHandleInternal obj, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectHandleInternal* outRetValue);
	};

}}}

#endif
