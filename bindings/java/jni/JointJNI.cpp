#include <joint/devkit/ScopeExit.hpp>

#include <JointJNI.h>
#include <binding/Object.hpp>
#include <utils/JPtr.hpp>
#include <utils/Utils.hpp>


using namespace joint::devkit;
using namespace joint_java;
using namespace joint_java::binding;

JOINT_DEVKIT_LOGGER("Joint.Java.JNI")

JNIEXPORT void JNICALL Java_org_joint_JointObject_finalize(JNIEnv* env, jobject obj)
{
	printf("JNI FINALIZE\n");
	JLocalClassPtr cls(env, env->GetObjectClass(obj));
	jfieldID handle_id = env->GetFieldID(cls, "handle", "J");
	jlong handle_long = env->GetLongField(obj, handle_id);
	Joint_ObjectHandle handle = (Joint_ObjectHandle)handle_long;
	printf("JNI FINALIZE handle: %p\n", handle);
	Joint_DecRefObject(handle);
}

JNIEXPORT jobject JNICALL Java_org_joint_JointObject_invokeMethod(JNIEnv* env, jobject obj, jint methodId, jobjectArray params)
{
	JNI_WRAP_CPP_BEGIN

	JLocalClassPtr cls(env, env->GetObjectClass(obj));
	jfieldID handle_id = env->GetFieldID(cls, "handle", "J");
	jlong handle_long = env->GetLongField(obj, handle_id);
	Joint_ObjectHandle handle = (Joint_ObjectHandle)handle_long;

	printf("JNI invokeMethod, handle: %p, methodId: %d\n", handle, (int)methodId);

	JNI_WRAP_CPP_END(NULL, NULL);
}


JNIEXPORT jobject JNICALL Java_org_joint_ModuleContext_register(JNIEnv* env, jobject moduleContext, jobject accessor)
{
	JNI_WRAP_CPP_BEGIN

	JLocalClassPtr cls(env, env->GetObjectClass(moduleContext));
	jfieldID handle_id = env->GetFieldID(cls, "handle", "J");
	jlong module_handle_long = env->GetLongField(moduleContext, handle_id);

	printf("JNI ModuleContext.register, handle: %p\n", (void*)module_handle_long);

	JavaVM* jvm = NULL;
	JNI_CALL( env->GetJavaVM(&jvm) );

	std::unique_ptr<Object> o(new Object(JGlobalObjPtr(jvm, accessor)));
	Joint_ObjectHandle handle = JOINT_NULL_HANDLE;
	Joint_ModuleHandle module = (Joint_ModuleHandle)module_handle_long;
	Joint_Error ret = Joint_CreateObject(module, o.get(), &handle);
	JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
	o.release();
	auto sg = ScopeExit([&]{ Joint_DecRefObject(handle); });

	printf("JNI New object handle: %p\n", handle);

	JLocalClassPtr obj_cls(jvm, env->FindClass("org/joint/JointObject"));
	JOINT_CHECK(obj_cls, StringBuilder() % "Class org/joint/JointObject not found");
	jmethodID obj_ctor_id = env->GetMethodID(obj_cls, "<init>", "(J)V");

	// TODO: Use JLocalObjPtr?
	jobject obj = env->NewObject(obj_cls, obj_ctor_id, (jlong)handle);
	sg.Cancel();

	JNI_WRAP_CPP_END(obj, NULL);
}
