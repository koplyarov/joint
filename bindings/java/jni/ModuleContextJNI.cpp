#include <joint/devkit/accessors/MakeAccessor.hpp>

#include <JointJNI.h>
#include <binding/JointJavaContext.hpp>
#include <binding/Object.hpp>
#include <jni/JNIHelpers.hpp>


using namespace joint::devkit;
using namespace joint::java;
using namespace joint::java::binding;

JOINT_DEVKIT_LOGGER("Joint.Java.JNI")


JNIEXPORT jobject JNICALL Java_org_joint_ModuleContext_doRegister(JNIEnv* env, jclass cls, jlong accessorVTableLong, jlong accessorInstanceLong, jobject jaccessor)
{
	JNI_WRAP_CPP_BEGIN

	JavaVM* jvm = nullptr;
	JNI_CALL( env->GetJavaVM(&jvm) );

	JointCore_ObjectAccessor native_accessor = accessors::MakeAccessor<binding::Object>(env, JObjGlobalRef::StealLocal(env, jaccessor));
	JObjLocalRef result = JointJavaContext::JointObject::Make(env, native_accessor);

	JNI_WRAP_CPP_END(result.Release(), 0)
}
