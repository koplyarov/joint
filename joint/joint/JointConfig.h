#ifndef JOINT_JOINTCONFIG_H
#define JOINT_JOINTCONFIG_H

#if defined(__GNUC__) || defined(__clang)
#	if !defined(JOINT_PLATFORM_POSIX)
#		define JOINT_PLATFORM_POSIX 1
#	endif
#endif

#if defined(_MSC_VER)
#	if !defined(JOINT_PLATFORM_WINDOWS)
#		define JOINT_PLATFORM_WINDOWS 1
#	endif
#endif

#endif
