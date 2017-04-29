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


	JointCore_Error Binding::Deinit(void* bindingUserData)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<Binding*>(bindingUserData);
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::LoadModule(void* bindingUserData, JointCore_ManifestHandle moduleManifest, JointCore_ModuleHandleInternal* outModule)
	{
		JOINT_CPP_WRAP_BEGIN

		ModuleManifest m;
		ManifestReader::Read(moduleManifest, m);

		*outModule = new Module(m, ManifestReader::GetLocation(moduleManifest));

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::UnloadModule(void* bindingUserData, JointCore_ModuleHandleInternal module)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<Module*>(module);
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::GetRootObject(JointCore_ModuleHandle module, void* bindingUserData, JointCore_ModuleHandleInternal moduleInt, const char* getterName, JointCore_ObjectHandle* outObject)
	{
		JOINT_CPP_WRAP_BEGIN

		auto m = reinterpret_cast<Module*>(moduleInt);

		auto jvm = JointJavaContext::GetJvm();
		auto env = GetJavaEnv(jvm);

		const auto& cls = m->GetClass();

		auto jm = JointJavaContext::ModuleContext::Make(env, module);

		jmethodID root_obj_getter_id = JAVA_CALL(env->GetStaticMethodID(cls.Get(), getterName, "(Lorg/joint/ModuleContext;)Lorg/joint/JointObject;"));

		auto root_obj = JObjLocalRef::StealLocal(env, JAVA_CALL(env->CallStaticObjectMethod(cls.Get(), root_obj_getter_id, jm.Get())));
		JointJavaContext::JointObject jo(root_obj);

		*outObject = jo.GetHandle();
		Joint_IncRefObject(*outObject);

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::InvokeMethod(JointCore_ModuleHandle module, void* bindingUserData, JointCore_ModuleHandleInternal moduleInt, JointCore_ObjectHandleInternal obj, JointCore_SizeT methodId, const JointCore_Parameter* params, JointCore_SizeT paramsCount, JointCore_Type retType, JointCore_RetValue* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN

		auto o = reinterpret_cast<Object*>(obj);
		return o->InvokeMethod(methodId, joint::ArrayView<const JointCore_Parameter>(params, paramsCount), retType, outRetValue);

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::ReleaseObject(void* bindingUserData, JointCore_ModuleHandleInternal module, JointCore_ObjectHandleInternal obj)
	{
		JOINT_CPP_WRAP_BEGIN

		delete reinterpret_cast<Object*>(obj);

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::CastObject(void* bindingUserData, JointCore_ModuleHandleInternal module, JointCore_ObjectHandleInternal obj, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectHandleInternal* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN

		auto jvm = JointJavaContext::GetJvm();
		auto env = GetJavaEnv(jvm);

		auto o = reinterpret_cast<Object*>(obj);

		auto iid = JointJavaContext::InterfaceId::Make(env, JStringLocalRef::StealLocal(env, JAVA_CALL(env->NewStringUTF(interfaceId))));

		auto accessor = o->GetAccessor(env);
		JObjLocalRef new_accessor = JointJavaContext::Accessor(accessor).Cast(iid);
		if (!new_accessor)
			return JOINT_CORE_ERROR_CAST_FAILED;

		*outRetValue = new Object(env, new_accessor.Global());

		JOINT_CPP_WRAP_END
	}

}}}
