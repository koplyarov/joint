#ifndef UTILS_JNIERROR_HPP
#define UTILS_JNIERROR_HPP


#include <joint/devkit/util/StringBuilder.hpp>

#include <jni.h>


namespace joint {
namespace java
{

    const char* JniErrorToString(jint err);

#define JNI_CALL(...) do { jint __res__ = __VA_ARGS__; if (__res__ != 0) JOINT_DEVKIT_THROW(joint::devkit::StringBuilder() % "JNI call " #__VA_ARGS__ " error: " % JniErrorToString(__res__)); } while (false)
}}

#endif
