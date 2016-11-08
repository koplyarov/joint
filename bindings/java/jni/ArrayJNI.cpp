#include <JointJNI.h>
#include <utils/Utils.hpp>


using namespace joint::java;

JOINT_DEVKIT_LOGGER("Joint.Java.JNI")


JNIEXPORT jlong JNICALL Java_org_joint_Array_initNative(JNIEnv* env, jclass cls, jobject typeDescriptor, jlong size)
{
	JNI_WRAP_CPP_BEGIN
	JNI_WRAP_CPP_END(0, 0)
}


JNIEXPORT void JNICALL Java_org_joint_Array_deinitNative(JNIEnv* env, jclass cls, jlong handle)
{
	JNI_WRAP_CPP_BEGIN
	JNI_WRAP_CPP_END_VOID()
}


JNIEXPORT jobject JNICALL Java_org_joint_Array_doGet(JNIEnv* env, jclass cls, jlong handle, jlong index)
{
	JNI_WRAP_CPP_BEGIN
	JNI_WRAP_CPP_END(nullptr, nullptr)
}


JNIEXPORT void JNICALL Java_org_joint_Array_doSet(JNIEnv* env, jclass cls, jlong handle, jlong index, jobject value)
{
	JNI_WRAP_CPP_BEGIN
	JNI_WRAP_CPP_END_VOID()
}
