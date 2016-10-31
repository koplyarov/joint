#include <binding/Module.hpp>

#include <joint/devkit/JointException.hpp>

#include <utils/JavaVirtualMachine.hpp>
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
		auto jvm = JavaVirtualMachine::GetJvm();
		auto env = GetJavaEnv(jvm);

		JLocalStringPtr url_string(env, reinterpret_cast<jstring>(JAVA_CALL(env->NewStringUTF(jarPath.c_str()))));
		JLocalStringPtr class_name_string(env, reinterpret_cast<jstring>(JAVA_CALL(env->NewStringUTF(className.c_str()))));

		JLocalClassPtr URL_cls(env, JAVA_CALL(env->FindClass("java/net/URL")));
		jmethodID URL_ctor_id = JAVA_CALL(env->GetMethodID(URL_cls.Get(), "<init>", "(Ljava/lang/String;)V"));
		JLocalObjPtr url(env, JAVA_CALL(env->NewObject(URL_cls.Get(), URL_ctor_id, url_string.Get())));

		JLocalObjArrayPtr urls(env, JAVA_CALL(env->NewObjectArray(1, URL_cls.Get(), url.Get())));

		JLocalClassPtr URLClassLoader_cls(env, JAVA_CALL(env->FindClass("java/net/URLClassLoader")));
		jmethodID URLClassLoader_ctor_id = JAVA_CALL(env->GetMethodID(URLClassLoader_cls.Get(), "<init>", "([Ljava/net/URL;)V"));
		jmethodID loadClass_id = JAVA_CALL(env->GetMethodID(URLClassLoader_cls.Get(), "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;"));

		JLocalObjPtr class_loader(env, JAVA_CALL(env->NewObject(URLClassLoader_cls.Get(), URLClassLoader_ctor_id, urls.Get())));

		_cls = JGlobalClassPtr(env, reinterpret_cast<jclass>(JAVA_CALL(env->CallObjectMethod(class_loader.Get(), loadClass_id, class_name_string.Get()))));
		JOINT_CHECK(_cls, StringBuilder() % "Class " % className % " not found");
	}


	Module::~Module()
	{ }

}}}
