#ifndef JOINT_DEVKIT_CONFIG_HPP
#define JOINT_DEVKIT_CONFIG_HPP

#if defined(__GNUC__) || defined(__clang)
#   if !defined(__EXCEPTIONS) && !defined(JOINT_DEVKIT_NOEXCEPTIONS)
#       define JOINT_DEVKIT_NOEXCEPTIONS 1
#   endif
#endif

#if defined(_MSC_VER)
#   if !defined(_CPPUNWIND) && !defined(JOINT_DEVKIT_NOEXCEPTIONS)
#       define JOINT_DEVKIT_NOEXCEPTIONS 1
#   endif
#endif

#if defined(_MSC_VER) && (JOINT_DEVKIT_NOEXCEPTIONS || _MSC_VER < 1900)
#   define JOINT_DEVKIT_NOEXCEPT
#else
#   define JOINT_DEVKIT_NOEXCEPT noexcept
#endif

#if defined(_MSC_VER)
#   define JOINT_DEVKIT_CHAR16_T wchar_t
#else
#   define JOINT_DEVKIT_CHAR16_T char16_t
#endif

#if !defined(JOINT_DEVKIT_DEBUG)
#   if defined(NDEBUG)
#       define JOINT_DEVKIT_DEBUG 0
#   else
#       define JOINT_DEVKIT_DEBUG 1
#   endif
#endif

#endif
