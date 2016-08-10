#ifndef JOINT_JOINTCONFIG_H
#define JOINT_JOINTCONFIG_H

#if defined(__GNUC__) || defined(__clang)
#	if !defined(JOINT_PLATFORM_POSIX)
#		define JOINT_PLATFORM_POSIX 1
#	endif
#	if !defined JOINT_WARN_UNUSED_RESULT
#		define JOINT_WARN_UNUSED_RESULT(Result_) Result_ __attribute__((warn_unused_result))
#	endif
#endif

#if defined(_MSC_VER)
#	if !defined(JOINT_PLATFORM_WINDOWS)
#		define JOINT_PLATFORM_WINDOWS 1
#	endif
#	if !defined JOINT_WARN_UNUSED_RESULT
#		define JOINT_WARN_UNUSED_RESULT(Result_) _Check_return_ Result_
#	endif
#endif

#if !defined JOINT_WARN_UNUSED_RESULT
#	define JOINT_WARN_UNUSED_RESULT(Result_) Result_
#endif

#endif
