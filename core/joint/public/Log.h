#ifndef JOINT_LOG_H
#define JOINT_LOG_H


#include <joint/public/Error.h>
#include <joint/public/JointConfig.h>
#include <joint/public/JointCoreApi.h>


JOINT_CORE_EXTERN_C_BEGIN

	typedef enum
	{
		JOINT_CORE_LOGLEVEL_DEBUG    = 0,
		JOINT_CORE_LOGLEVEL_INFO     = 1,
		JOINT_CORE_LOGLEVEL_WARNING  = 2,
		JOINT_CORE_LOGLEVEL_ERROR    = 3
	} JointCore_LogLevel;

	JOINT_CORE_API const char* JointCore_LogLevelToString(JointCore_LogLevel logLevel);

	typedef void JointCore_LogCallback_Func(JointCore_LogLevel logLevel, const char* category, const char* message);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_SetLogCallback(JointCore_LogCallback_Func* logCallback);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_SetLogLevel(JointCore_LogLevel logLevel);

	JOINT_CORE_API JointCore_LogLevel Joint_GetLogLevel();
	JOINT_CORE_API void Joint_Log(JointCore_LogLevel logLevel, const char* subsystem, const char* format, ...);

JOINT_CORE_EXTERN_C_END

#endif
