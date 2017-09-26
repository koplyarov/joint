#include <joint/public/Assert.h>

#include <joint/public/Log.h>

#include <stdexcept>

#include <stdarg.h>
#include <stdio.h>


extern "C"
{

	void JointCore_FatalImpl(const char* subsystem, const char* file, int line, const char* func, const char* format, ...)
	{
		char message[1024];
		va_list argptr;
		va_start(argptr, format);
		vsnprintf(message, sizeof(message), format, argptr);
		va_end(argptr);

		Joint_Log(JOINT_CORE_LOGLEVEL_ERROR, subsystem, "Fatal condition at %s:%d in function %s: %s", file, line, func, message);

		std::terminate();
	}

}
