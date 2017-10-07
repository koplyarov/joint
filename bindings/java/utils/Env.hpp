#ifndef UTILS_ENV_HPP
#define UTILS_ENV_HPP


#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/util/Exceptions.hpp>

#include <jni.h>


namespace joint {
namespace java
{

	inline JNIEnv* GetJavaEnv(JavaVM* jvm, int version = JNI_VERSION_1_6)
	{
		JOINT_DEVKIT_FUNCTION_LOCAL_LOGGER("Joint.Java.Utils");

		thread_local JNIEnv* env = nullptr;
		if (!env)
		{
			int retcode = jvm->GetEnv((void **)&env, version);
			if (retcode == JNI_EDETACHED)
				JOINT_THROW("Java VM is not attached to the current thread!");
			if (retcode == JNI_EVERSION)
				JOINT_THROW(devkit::StringBuilder() % "Java VM does not support version " % (version >> 16) % "." % ((version >> 8) & 0xFF) % "." % (version & 0xFF));
			if (retcode != JNI_OK)
				JOINT_THROW(devkit::StringBuilder() % "Cannot get JNIEnv from Java VM, retcode: " % retcode);
		}
		return env;
	}

}}

#endif
