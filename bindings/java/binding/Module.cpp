#include <binding/Module.hpp>

#include <joint/utils/JointException.hpp>

#include <utils/JniError.hpp>


namespace joint_java {
namespace binding
{

	using namespace joint::devkit;


	Module::Module(const std::string& jarPath, const std::string& className)
		: _jvm(), _cls()
	{
		std::string jar_path_opt = "-Djava.class.path=" + jarPath;
		JavaVMOption opt[] = { { const_cast<char*>(jar_path_opt.c_str()), nullptr } };

		JavaVMInitArgs vm_args = { };
		vm_args.version = 0x00010001;
		JNI_GetDefaultJavaVMInitArgs(&vm_args);
		vm_args.options = opt;
		vm_args.nOptions = sizeof(opt) / sizeof(opt[0]);

		JNIEnv* env;
		jint ret = JNI_CreateJavaVM(&_jvm, reinterpret_cast<void**>(&env), &vm_args);
		JOINT_CHECK(ret == 0, StringBuilder() % "JNI_CreateJavaVM failed: " % JniErrorToString(ret));
		JOINT_CHECK(_jvm, "JNI_CreateJavaVM failed!");

		_cls = JGlobalClassPtr(_jvm, env->FindClass(className.c_str()));
		JOINT_CHECK(_cls, StringBuilder() % "Class " % className % " not found");
	}


	Module::~Module()
	{
		GetLogger().Warning() << "Module dtor";
		_cls.Reset();
		_jvm->DestroyJavaVM();
	}

}}
