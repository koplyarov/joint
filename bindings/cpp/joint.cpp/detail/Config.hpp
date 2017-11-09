#ifndef JOINT_CPP_DETAIL_CONFIG_HPP
#define JOINT_CPP_DETAIL_CONFIG_HPP

#if __cplusplus > 201100L
#   define DETAIL_JOINT_CPP_MOVE_SUPPORTED 1
#   define DETAIL_JOINT_CPP_STATIC_ASSERT_SUPPORTED 1
#else
#   define DETAIL_JOINT_CPP_MOVE_SUPPORTED 0
#   define DETAIL_JOINT_CPP_STATIC_ASSERT_SUPPORTED 0
#endif


#if defined(__GNUC__) || defined(__clang)
#   if !defined(__EXCEPTIONS) && !defined(JOINT_CPP_CONFIG_NO_EXCEPTIONS)
#       define JOINT_CPP_CONFIG_NO_EXCEPTIONS 1
#   endif
#endif

#if defined(_MSC_VER)
#   if !defined(_CPPUNWIND) && !defined(JOINT_CPP_CONFIG_NO_EXCEPTIONS)
#       define JOINT_CPP_CONFIG_NO_EXCEPTIONS 1
#   endif
#endif


#if defined(_MSC_VER) && (JOINT_CPP_CONFIG_NO_EXCEPTIONS || _MSC_VER < 1900)
#   define JOINT_DEVKIT_NOEXCEPT
#else
#   define JOINT_DEVKIT_NOEXCEPT noexcept
#endif

#endif
