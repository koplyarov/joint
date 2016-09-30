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
		GetLogger().Warning() << "Module ctor";

		std::string class_path_opt = "-Djava.class.path=" + jarPath + ":/home/koplyarov/work/joint/build/bin/joint.jar";
		std::string lib_path_opt = "-Djava.library.path=/home/koplyarov/work/joint/build/bin";
		JavaVMOption opt[] = {
			{ const_cast<char*>(class_path_opt.c_str()), nullptr },
			{ const_cast<char*>(lib_path_opt.c_str()), nullptr }
		};

		JavaVMInitArgs vm_args = { };
		vm_args.version = 0x00010001;
		JNI_GetDefaultJavaVMInitArgs(&vm_args);
		vm_args.options = opt;
		vm_args.nOptions = sizeof(opt) / sizeof(opt[0]);

		JNIEnv* env = NULL;
		JavaVM* jvm = NULL;
		jint ret = JNI_CreateJavaVM(&jvm, reinterpret_cast<void**>(&env), &vm_args);
		JOINT_CHECK(ret == 0, StringBuilder() % "JNI_CreateJavaVM failed: " % JniErrorToString(ret));
		JOINT_CHECK(jvm, "JNI_CreateJavaVM failed!");

		// TODO: DestroyJavaVM does not collect the garbage (and its native parts!). Fix it.
		_jvm = JavaVMHolder(jvm, [](JavaVM* jvm) { jvm->DestroyJavaVM(); });

		_cls = JGlobalClassPtr(_jvm.Get(), env->FindClass(className.c_str()));
		JOINT_CHECK(_cls, StringBuilder() % "Class " % className % " not found");
	}


	Module::~Module()
	{
		GetLogger().Warning() << "Module dtor";
	}

}}
