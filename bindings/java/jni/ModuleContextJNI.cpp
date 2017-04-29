#include <JointJNI.h>
#include <binding/Object.hpp>


using namespace joint::devkit;
using namespace joint::java;
using namespace joint::java::binding;

JOINT_DEVKIT_LOGGER("Joint.Java.JNI")


JNIEXPORT jlong JNICALL Java_org_joint_ModuleContext_doRegister(JNIEnv* env, jclass cls, jlong moduleHandleLong, jobject accessor)
{
	JNI_WRAP_CPP_BEGIN

	JavaVM* jvm = nullptr;
	JNI_CALL( env->GetJavaVM(&jvm) );

	std::unique_ptr<Object> o(new Object(env, JObjGlobalRef::StealLocal(env, accessor)));
	JointCore_ObjectHandle handle = JOINT_CORE_NULL_HANDLE;
	JointCore_ModuleHandle module = (JointCore_ModuleHandle)moduleHandleLong;
	JointCore_Error ret = Joint_CreateObject(module, o.get(), &handle);
	JOINT_CHECK(ret == JOINT_CORE_ERROR_NONE, ret);
	o.release();

	JNI_WRAP_CPP_END(reinterpret_cast<jlong>(handle), 0)
}
