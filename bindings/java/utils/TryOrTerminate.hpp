#ifndef UTILS_TRYORTERMINATE_HPP
#define UTILS_TRYORTERMINATE_HPP


#include <joint/devkit/util/JointException.hpp>

#include <jni.h>


namespace joint {
namespace java {
namespace Detail
{

	template < typename T_ >
	T_ TryOrTerminate(T_ val, JNIEnv* env, const char* LoggerName, const char* msg)
	{
		if (env->ExceptionCheck())
		{
			env->ExceptionDescribe();
			JOINT_TERMINATE(msg);
		}

		return val;
	}

}}}

#define JOINT_JAVA_TRY_OR_TERMINATE(...) Detail::TryOrTerminate(__VA_ARGS__, env, GetLogger().GetName(), #__VA_ARGS__ " failed")


#endif
