#include <joint/public/Log.h>

#include <joint/devkit/CppWrappers.hpp>
#include <joint/devkit/Logger.hpp>

#include <atomic>
#include <mutex>
#include <string.h>

#include <stdarg.h>
#include <stdio.h>


#if JOINT_PLATFORM_POSIX
#	include <unistd.h>
#endif


static const char* LoggerName = "Joint.Core";
JOINT_DEVKIT_LOGGER(LoggerName)


static void DefaultLogCallback(JointCore_LogLevel logLevel, const char* subsystem, const char* message)
{
	const char* alignment = "        ";
	const char* log_level_str = JointCore_LogLevelToString(logLevel);
#if JOINT_PLATFORM_POSIX
	if (isatty(2))
	{
		const char* subsystem_ns_color = "\33[0;32m";
		const char* subsystem_name_color = "\33[1;32m";
		const char* log_color = "\33[0m";

		switch (logLevel)
		{
		case JOINT_CORE_LOGLEVEL_DEBUG:    log_color = "\33[2;37m";  break;
		case JOINT_CORE_LOGLEVEL_INFO:     log_color = "\33[0m";     break;
		case JOINT_CORE_LOGLEVEL_WARNING:  log_color = "\33[1;33m";  break;
		case JOINT_CORE_LOGLEVEL_ERROR:    log_color = "\33[1;31m";  break;
		}

		int subsystem_len = strlen(subsystem), subsystem_name_pos = subsystem_len;
		while (subsystem_name_pos > 0 && subsystem[subsystem_name_pos] != '.')
			--subsystem_name_pos;
		if (subsystem[subsystem_name_pos] == '.')
			++subsystem_name_pos;

		fprintf(stderr, "%s[%s]%.*s%s[%.*s%s%s%s] %s%s\33[0m\n",
			log_color, log_level_str, (int)(sizeof("Warning") - strlen(log_level_str)), alignment,
			subsystem_ns_color, subsystem_name_pos, subsystem, subsystem_name_color, subsystem + subsystem_name_pos, subsystem_ns_color,
			log_color, message);
	}
	else
#endif
		fprintf(stderr, "[%s]%.*s[%s] %s\n", JointCore_LogLevelToString(logLevel), (int)(sizeof("Warning") - strlen(log_level_str)), alignment, subsystem, message);
}


static std::mutex                     g_mutex;
static JointCore_LogCallback_Func*        g_logCallback = &DefaultLogCallback;
static std::atomic<JointCore_LogLevel>    g_logLevel(JOINT_CORE_LOGLEVEL_WARNING);


extern "C"
{

	const char* JointCore_LogLevelToString(JointCore_LogLevel logLevel)
	{
		switch (logLevel)
		{
		case JOINT_CORE_LOGLEVEL_DEBUG:    return "Debug";
		case JOINT_CORE_LOGLEVEL_INFO:     return "Info";
		case JOINT_CORE_LOGLEVEL_WARNING:  return "Warning";
		case JOINT_CORE_LOGLEVEL_ERROR:    return "Error";
		default:                      return "Unknown log level";
		}
	}

	JointCore_Error Joint_SetLogCallback(JointCore_LogCallback_Func* logCallback)
	{
		JOINT_CPP_WRAP_BEGIN
		std::lock_guard<std::mutex> l(g_mutex);
		g_logCallback = (logCallback == nullptr) ? &DefaultLogCallback : logCallback;
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_SetLogLevel(JointCore_LogLevel logLevel)
	{
		JOINT_CPP_WRAP_BEGIN
		JOINT_CHECK(logLevel >= JOINT_CORE_LOGLEVEL_DEBUG && logLevel <= JOINT_CORE_LOGLEVEL_ERROR, JOINT_CORE_ERROR_INVALID_PARAMETER);
		g_logLevel = logLevel;
		JOINT_CPP_WRAP_END
	}


	JointCore_LogLevel Joint_GetLogLevel()
	{ return g_logLevel.load(std::memory_order_relaxed); }


	void Joint_Log(JointCore_LogLevel logLevel, const char* subsystem, const char* format, ...)
	{
		if (logLevel < g_logLevel.load(std::memory_order_relaxed))
			return;

		char message[1024];
		va_list argptr;
		va_start(argptr, format);
		vsnprintf(message, sizeof(message), format, argptr);
		va_end(argptr);

		std::lock_guard<std::mutex> l(g_mutex);
		g_logCallback(logLevel, subsystem, message);
	}

}
