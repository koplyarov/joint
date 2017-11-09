#include <joint/devkit/marshalling/ExceptionInfo.hpp>

#include <JointJNI.h>
#include <utils/Utils.hpp>


using namespace joint::devkit;

JOINT_DEVKIT_LOGGER("Joint.Java.JNI")


JNIEXPORT void JNICALL Java_org_joint_JointException_deinitNative(JNIEnv* env, jclass cls, jlong nativeData)
{
    JNI_WRAP_CPP_BEGIN
    auto handle = reinterpret_cast<JointCore_Exception_Handle>(nativeData);
    JointCore_Exception_DecRef(handle);
    JNI_WRAP_CPP_END_VOID()
}
