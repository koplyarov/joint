#include <JointJNI.h>
#include <binding/JavaBindingInfo.hpp>


using namespace joint::devkit;
using namespace joint::java;

JOINT_DEVKIT_LOGGER("Joint.Java.JNI")


JNIEXPORT jlong JNICALL Java_org_joint_InterfaceDescriptor_initNative(JNIEnv* env, jobject self)
{
	JNI_WRAP_CPP_BEGIN
	env->NewLocalRef(self); // one new ref for JLocalObjPtr
	auto result = new JavaInterfaceDescriptor(JObjTempRef(env, self), JavaBindingInfo());
	JNI_WRAP_CPP_END(reinterpret_cast<jlong>(result), 0)
}


JNIEXPORT void JNICALL Java_org_joint_InterfaceDescriptor_deinitNative(JNIEnv* env, jclass cls, jlong native)
{
	JNI_WRAP_CPP_BEGIN
	delete reinterpret_cast<JavaInterfaceDescriptor*>(native);
	JNI_WRAP_CPP_END_VOID()
}
