#include <utils/JavaVirtualMachine.hpp>

#include <joint/devkit/JointException.hpp>

#include <utils/JniError.hpp>


namespace joint {
namespace java
{

	using namespace devkit;


	JavaVirtualMachine::JavaVirtualMachine()
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


	JavaVirtualMachine::~JavaVirtualMachine()
	{
		_jvm->DestroyJavaVM();
	}

}}
