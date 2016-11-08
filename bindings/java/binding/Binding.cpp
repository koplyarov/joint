#include <binding/Binding.hpp>

#include <joint/Joint.h>
#include <joint/devkit/CppWrappers.hpp>
#include <joint/devkit/Holder.hpp>

#include <memory>
#include <string.h>

#include <stdio.h>

#include <binding/JointJavaContext.hpp>
#include <binding/Module.hpp>
#include <binding/Object.hpp>
#include <jni.h>
#include <utils/Utils.hpp>


namespace joint {
namespace java {
namespace binding
{

	using namespace devkit;


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

		auto jvm = JointJavaContext::GetJvm();
		auto env = GetJavaEnv(jvm);

		const auto& cls = m->GetClass();

		auto jm = JointJavaContext::ModuleContext::Make(env, module);

		jmethodID root_obj_getter_id = JAVA_CALL(env->GetStaticMethodID(cls, getterName, "(Lorg/joint/ModuleContext;)Lorg/joint/JointObject;"));

		auto root_obj = JObjLocalRef::StealLocal(env, JAVA_CALL(env->CallStaticObjectMethod(cls, root_obj_getter_id, jm.Get())));
		JointJavaContext::JointObject jo(root_obj);

		*outObject = jo.GetHandle();
		Joint_IncRefObject(*outObject);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::InvokeMethod(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Parameter* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN

		auto o = reinterpret_cast<Object*>(obj);
		return o->InvokeMethod(methodId, joint::ArrayView<const Joint_Parameter>(params, paramsCount), retType, outRetValue);

		JOINT_CPP_WRAP_END
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

		auto jvm = JointJavaContext::GetJvm();
		auto env = GetJavaEnv(jvm);

		auto o = reinterpret_cast<Object*>(obj);

		auto iid = JointJavaContext::InterfaceId::Make(env, JStringLocalRef::StealLocal(env, JAVA_CALL(env->NewStringUTF(interfaceId))));

		auto accessor = o->GetAccessor(env);
		JObjLocalRef new_accessor = JointJavaContext::Accessor(accessor).Cast(iid);
		if (!new_accessor)
			return JOINT_ERROR_CAST_FAILED;

		*outRetValue = new Object(env, new_accessor.Global());

		JOINT_CPP_WRAP_END
	}

}}}
