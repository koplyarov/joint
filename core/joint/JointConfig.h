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

#ifdef _MSC_VER
#	define JOINT_EXPORT __declspec(dllexport)
#	define JOINT_IMPORT __declspec(dllimport)
#else
#	define JOINT_EXPORT
#	define JOINT_IMPORT
#endif


#if defined(__GNUC__) || defined(__clang)
#	if !defined(JOINT_LIKELY)
#		define JOINT_LIKELY(Expr_)        __builtin_expect(!!(Expr_), 1)
#	endif
#	if !defined(JOINT_UNLIKELY)
#		define JOINT_UNLIKELY(Expr_)      __builtin_expect((Expr_), 0)
#	endif
#else
#	if !defined(JOINT_LIKELY)
#		define JOINT_LIKELY(Expr_)        (Expr_)
#	endif
#	if !defined(JOINT_UNLIKELY)
#		define JOINT_UNLIKELY(Expr_)      (Expr_)
#	endif
#endif

#endif