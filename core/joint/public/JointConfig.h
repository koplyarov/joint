#ifndef JOINT_JOINTCONFIG_H
#define JOINT_JOINTCONFIG_H


#ifdef __cplusplus
#   define JOINT_CORE_EXTERN_C_BEGIN extern "C" {
#   define JOINT_CORE_EXTERN_C_END }
#   if defined(_MSC_VER) && _MSC_VER >= 1800
#       define JOINT_CORE_CPP_STANDARD 201100L
#   else
#       define JOINT_CORE_CPP_STANDARD __cplusplus
#   endif
#else
#   define JOINT_CORE_EXTERN_C_BEGIN
#   define JOINT_CORE_EXTERN_C_END
#endif

#if defined(__GNUC__) || defined(__clang)
#   if !defined(JOINT_PLATFORM_POSIX)
#       define JOINT_PLATFORM_POSIX 1
#   endif
#   if !defined JOINT_CORE_WARN_UNUSED_RESULT
#       define JOINT_CORE_WARN_UNUSED_RESULT(Result_) Result_ __attribute__((warn_unused_result))
#   endif
#endif

#if defined(_MSC_VER)
#   if !defined(JOINT_CORE_PLATFORM_WINDOWS)
#       define JOINT_CORE_PLATFORM_WINDOWS 1
#   endif
#endif

#if !defined JOINT_CORE_WARN_UNUSED_RESULT
#   define JOINT_CORE_WARN_UNUSED_RESULT(Result_) Result_
#endif

#ifdef _MSC_VER
#   define JOINT_CORE_EXPORT __declspec(dllexport)
#   define JOINT_CORE_IMPORT __declspec(dllimport)
#else
#   define JOINT_CORE_EXPORT
#   define JOINT_CORE_IMPORT
#endif


#if defined(__GNUC__) || defined(__clang)
#   if !defined(JOINT_CORE_LIKELY)
#       define JOINT_CORE_LIKELY(Expr_)        __builtin_expect(!!(Expr_), 1)
#   endif
#   if !defined(JOINT_CORE_UNLIKELY)
#       define JOINT_CORE_UNLIKELY(Expr_)      __builtin_expect((Expr_), 0)
#   endif
#else
#   if !defined(JOINT_CORE_LIKELY)
#       define JOINT_CORE_LIKELY(Expr_)        (Expr_)
#   endif
#   if !defined(JOINT_CORE_UNLIKELY)
#       define JOINT_CORE_UNLIKELY(Expr_)      (Expr_)
#   endif
#endif


#if !defined(JOINT_CORE_DEBUG)
#   if defined(NDEBUG)
#       define JOINT_CORE_DEBUG 0
#   else
#       define JOINT_CORE_DEBUG 1
#   endif
#endif


#define JOINT_CORE_UNUSED(Identifier_) (void)Identifier_

#endif
