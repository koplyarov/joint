#include <binding/Module.hpp>

#include <joint/devkit/JointException.hpp>

#include <binding/JointJavaContext.hpp>
#include <utils/JniError.hpp>
#include <utils/Utils.hpp>


namespace joint {
namespace java {
namespace binding
{

	using namespace devkit;


	Module::Module(const std::string& jarPath, const std::string& className)
		: _cls()
	{
		auto jvm = JointJavaContext::GetJvm();
		auto env = GetJavaEnv(jvm);

		auto url_string = JStringLocalRef::StealLocal(env, JAVA_CALL(env->NewStringUTF(jarPath.c_str())));
		auto class_name_string = JStringLocalRef::StealLocal(env, JAVA_CALL(env->NewStringUTF(className.c_str())));

		auto URL_cls = JClassLocalRef::StealLocal(env, JAVA_CALL(env->FindClass("java/net/URL")));
		jmethodID URL_ctor_id = JAVA_CALL(env->GetMethodID(URL_cls, "<init>", "(Ljava/lang/String;)V"));
		auto url = JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(URL_cls, URL_ctor_id, url_string.Get())));

		auto urls = JObjArrayLocalRef::StealLocal(env, JAVA_CALL(env->NewObjectArray(1, URL_cls, url)));

		auto URLClassLoader_cls = JClassLocalRef::StealLocal(env, JAVA_CALL(env->FindClass("java/net/URLClassLoader")));
		jmethodID URLClassLoader_ctor_id = JAVA_CALL(env->GetMethodID(URLClassLoader_cls, "<init>", "([Ljava/net/URL;)V"));
		jmethodID loadClass_id = JAVA_CALL(env->GetMethodID(URLClassLoader_cls, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;"));

		auto class_loader = JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(URLClassLoader_cls, URLClassLoader_ctor_id, urls.Get())));

		_cls = JGlobalClassPtr(env, reinterpret_cast<jclass>(JAVA_CALL(env->CallObjectMethod(class_loader, loadClass_id, class_name_string.Get()))));
		JOINT_CHECK(_cls, StringBuilder() % "Class " % className % " not found");
	}


	Module::~Module()
	{ }

}}}
