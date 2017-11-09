#ifndef JOINT_UTIL_ASSERT_H
#define JOINT_UTIL_ASSERT_H


#include <joint/public/JointConfig.h>
#include <joint/public/JointCoreApi.h>


JOINT_CORE_EXTERN_C_BEGIN

JOINT_CORE_API void JointCore_FatalImpl(const char* subsystem, const char* file, int line, const char* func, const char* format, ...);


#define JOINT_CORE_FATAL(Subsystem_, ...) \
        do { \
            JointCore_FatalImpl( \
                (Subsystem_), __FILE__, __LINE__, __func__, \
                __VA_ARGS__ \
            ); \
        } while (0)


#if JOINT_CORE_DEBUG
#   define JOINT_CORE_ASSERT(Expr_) \
        do { \
            if (!(Expr_)) \
                JointCore_FatalImpl( \
                    "Joint.Core", __FILE__, __LINE__, __func__, \
                    "Assertion failed at %s:%d in function %s\n%s", \
                    #Expr_ \
                ); \
        } while (0)
#else
#   define JOINT_CORE_ASSERT(Expr_) (void)(Expr_)
#endif

JOINT_CORE_EXTERN_C_END

#endif
