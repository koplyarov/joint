#ifndef TEST_CORE_UTILS_CONFIG_HPP
#define TEST_CORE_UTILS_CONFIG_HPP

#if defined(__GNUC__) || defined(__clang)
#	if !defined(TEST_PLATFORM_POSIX)
#		define TEST_PLATFORM_POSIX 1
#	endif
#endif

#if defined(_MSC_VER)
#	if !defined(TEST_PLATFORM_WINDOWS)
#		define TEST_PLATFORM_WINDOWS 1
#	endif
#endif

#endif
