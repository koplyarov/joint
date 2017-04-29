#include <joint/devkit/InterfaceDescriptor.hpp>
#include <joint/devkit/Utils.hpp>
#include <joint/devkit/ValueMarshaller.hpp>

#include <JointJNI.h>
#include <binding/Boxing.hpp>
#include <binding/JavaBindingInfo.hpp>
#include <binding/Marshaller.hpp>
#include <utils/Utils.hpp>


using namespace joint::devkit;
using namespace joint::java;

JOINT_DEVKIT_LOGGER("Joint.Java.JNI")


JNIEXPORT jlong JNICALL Java_org_joint_Array_initNative(JNIEnv* env, jclass cls, jobject typeDescriptor, jlong size)
{
	JNI_WRAP_CPP_BEGIN
	TypeDescriptor<JavaBindingInfo> td(JObjTempRef(env, typeDescriptor), JavaBindingInfo());

	JointCore_ArrayHandle handle = JOINT_CORE_NULL_HANDLE;
	JointCore_Error ret = Joint_MakeArray(td.GetJointType(), NoOverflowCast<JointCore_SizeT>(size), &handle);
	JOINT_CHECK(ret == JOINT_CORE_ERROR_NONE, ret);

	JNI_WRAP_CPP_END(reinterpret_cast<jlong>(handle), 0)
}


JNIEXPORT void JNICALL Java_org_joint_Array_deinitNative(JNIEnv* env, jclass cls, jobject typeDescriptor, jlong handleLong)
{
	JNI_WRAP_CPP_BEGIN

	auto handle = reinterpret_cast<JointCore_ArrayHandle>(handleLong);
	Joint_DecRefArray(handle);

	JNI_WRAP_CPP_END_VOID()
}


JNIEXPORT jobject JNICALL Java_org_joint_Array_doGet(JNIEnv* env, jclass cls, jobject typeDescriptor, jlong handleLong, jlong index)
{
	JNI_WRAP_CPP_BEGIN

	auto handle = reinterpret_cast<JointCore_ArrayHandle>(handleLong);

	JointCore_Value value;
	JointCore_Error ret = Joint_ArrayGet(handle, NoOverflowCast<JointCore_SizeT>(index), &value);
	JOINT_CHECK(ret == JOINT_CORE_ERROR_NONE, ret);

	TypeDescriptor<JavaBindingInfo> td(JObjTempRef(env, typeDescriptor), JavaBindingInfo());
	jvalue v = ValueMarshaller::FromJoint<jvalue>(ValueDirection::Return, td, value, JavaMarshaller(env));
	auto result = Boxing(env).Box(td.GetJointType(), v);

	JNI_WRAP_CPP_END(result.Release(), nullptr)
}


JNIEXPORT void JNICALL Java_org_joint_Array_doSet(JNIEnv* env, jclass cls, jlong handle, jlong index, jobject value)
{
	JNI_WRAP_CPP_BEGIN
	JOINT_THROW(JOINT_CORE_ERROR_NOT_IMPLEMENTED);
	JNI_WRAP_CPP_END_VOID()
}
