#include <utils/Utils.hpp>

#include <joint/devkit/util/StringBuilder.hpp>


namespace joint {
namespace java
{

	void ThrowExceptionFromJava(JNIEnv* env, const char* location)
	{
		throw std::runtime_error(devkit::StringBuilder() % "Java exception at " % location % ": " % GetJavaExceptionInfo(env).ToString());
	}

}}
