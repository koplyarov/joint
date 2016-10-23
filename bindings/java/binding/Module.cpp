#include <binding/Module.hpp>

#include <joint/devkit/JointException.hpp>

#include <utils/JniError.hpp>
#include <utils/Utils.hpp>


namespace joint {
namespace java {
namespace binding
{

	using namespace devkit;


	class JavaVirtualMachine
	{
		JOINT_DEVKIT_LOGGER("Joint.Java.JavaVirtualMachine");

	private:
		JavaVM*     _jvm;

	public:
		JavaVirtualMachine()
		{
			std::string class_path_opt = "-Djava.class.path=/home/koplyarov/work/joint/build/bin/joint.jar";
			std::string lib_path_opt = "-Djava.library.path=/home/koplyarov/work/joint/build/bin";
			JavaVMOption opt[] = {
				{ const_cast<char*>(class_path_opt.c_str()), nullptr },
				{ const_cast<char*>(lib_path_opt.c_str()), nullptr }
			};

			JavaVMInitArgs vm_args = { };
			vm_args.version = 0x00010006;
			jint ret = JNI_GetDefaultJavaVMInitArgs(&vm_args);
			JOINT_CHECK(ret == 0, StringBuilder() % "JNI_GetDefaultJavaVMInitArgs failed: " % JniErrorToString(ret));
			vm_args.options = opt;
			vm_args.nOptions = sizeof(opt) / sizeof(opt[0]);

			JNIEnv* env = NULL;
			ret = JNI_CreateJavaVM(&_jvm, reinterpret_cast<void**>(&env), &vm_args);
			JOINT_CHECK(ret == 0, StringBuilder() % "JNI_CreateJavaVM failed: " % JniErrorToString(ret));
			JOINT_CHECK(_jvm, "JNI_CreateJavaVM failed!");
		}

		~JavaVirtualMachine()
		{
			_jvm->DestroyJavaVM();
		}

		JavaVM* GetJvm() const { return _jvm; }
	};


	Module::Module(const std::string& jarPath, const std::string& className)
		: _cls()
	{
		static JavaVirtualMachine java_virtual_machine;
		auto jvm = java_virtual_machine.GetJvm();
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
