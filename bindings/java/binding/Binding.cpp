#include <binding/Binding.hpp>

#include <joint/Joint.h>
#include <joint/devkit/Holder.hpp>
#include <joint/utils/CppWrappers.hpp>

#include <memory>
#include <string.h>

#include <stdio.h>

#include <binding/Module.hpp>
#include <binding/Object.hpp>
#include <jni.h>


namespace joint_java {
namespace binding
{

	using namespace joint::devkit;


	Binding::Binding()
	{ GetLogger().Debug() << "Created"; }


	Binding::~Binding()
	{ GetLogger().Debug() << "Destroying"; }


	Joint_Error Binding::Deinit(void* bindingUserData)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<Binding*>(bindingUserData);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::LoadModule(void* bindingUserData, const char* moduleName, Joint_ModuleHandleInternal* outModule)
	{
		JOINT_CPP_WRAP_BEGIN

		std::string module_name_str(moduleName);
		size_t delim = module_name_str.rfind(':');
		JOINT_CHECK(delim != std::string::npos, JOINT_ERROR_INVALID_PARAMETER);
		std::string jar_path = module_name_str.substr(0, delim);
		std::string class_name = module_name_str.substr(delim + 1);

		std::unique_ptr<Module> m(new Module(jar_path, class_name));
		*outModule = m.release();

		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::UnloadModule(void* bindingUserData, Joint_ModuleHandleInternal module)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<Module*>(module);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::GetRootObject(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, const char* getterName, Joint_ObjectHandle* outObject)
	{
		JOINT_CPP_WRAP_BEGIN

		auto m = reinterpret_cast<Module*>(moduleInt);

		const auto& cls = m->GetClass();
		auto env = cls.GetEnv();
		auto jvm = cls.GetJvm();

		JLocalClassPtr module_cls(jvm, env->FindClass("org/joint/ModuleContext"));
		JOINT_CHECK(module_cls, StringBuilder() % "Class org/joint/ModuleContext not found");
		jmethodID module_ctx_ctor_id = env->GetMethodID(module_cls, "<init>", "(J)V");
		JLocalObjPtr module_ctx(jvm, env->NewObject(module_cls, module_ctx_ctor_id, (jlong)module));

		jmethodID root_obj_getter_id = env->GetStaticMethodID(cls, getterName, "(Lorg/joint/ModuleContext;)Lorg/joint/JointObject;");
		JOINT_CHECK(root_obj_getter_id, StringBuilder() % "Method " % getterName % " not found");
		JLocalObjPtr obj(jvm, env->CallStaticObjectMethod(cls, root_obj_getter_id, module_ctx.Get()));

		JLocalClassPtr obj_cls(jvm, env->FindClass("org/joint/JointObject"));
		JOINT_CHECK(obj_cls, StringBuilder() % "Class org/joint/JointObject not found");
		jfieldID handle_id = env->GetFieldID(obj_cls, "handle", "J");
		jlong handle_long = env->GetLongField(obj.Get(), handle_id);

		*outObject = (Joint_ObjectHandle)handle_long;
		Joint_IncRefObject(*outObject);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::InvokeMethod(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Parameter* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
	{
		// Nothing here may throw, so there is no JOINT_CPP_WRAP macros. This improves performance
		return JOINT_ERROR_NOT_IMPLEMENTED;
		//auto accessor = reinterpret_cast<JointC_Accessor*>(obj);
		//return accessor->VTable->InvokeMethod(accessor->Component, methodId, params, paramsCount, retType, outRetValue);
	}


	Joint_Error Binding::ReleaseObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj)
	{
		JOINT_CPP_WRAP_BEGIN

		delete reinterpret_cast<Object*>(obj);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::CastObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, Joint_InterfaceId interfaceId, Joint_InterfaceChecksum checksum, Joint_ObjectHandleInternal* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_THROW("Not implemented");
		//auto accessor = reinterpret_cast<JointC_Accessor*>(obj);
		//const JointC_Accessor* result = nullptr;
		//Joint_Error ret = accessor->VTable->CastObject(accessor->Component, interfaceId, checksum, &result);
		//*outRetValue = const_cast<JointC_Accessor*>(result);
		//return ret;

		JOINT_CPP_WRAP_END
	}

}}
