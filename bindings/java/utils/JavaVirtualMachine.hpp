#ifndef UTILS_JAVAVIRTUALMACHINE_HPP
#define UTILS_JAVAVIRTUALMACHINE_HPP


#include <joint/devkit/Logger.hpp>
#include <joint/devkit/Singleton.hpp>

#include <jni.h>


namespace joint {
namespace java
{

	class JavaVirtualMachine : private devkit::Singleton<JavaVirtualMachine>
	{
		JOINT_DEVKIT_SINGLETON_INTERNALS(JavaVirtualMachine);
		JOINT_DEVKIT_LOGGER("Joint.Java.JavaVirtualMachine");

	private:
		JavaVM*     _jvm;

	public:
		~JavaVirtualMachine();

		static JavaVM* GetJvm() { return Instance()._jvm; }

	private:
		JavaVirtualMachine();
	};

}}

#endif
