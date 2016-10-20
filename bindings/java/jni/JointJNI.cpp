#include <joint/devkit/ScopeExit.hpp>

#include <vector>

#include <JointJNI.h>
#include <binding/JavaBindingInfo.hpp>
#include <binding/Object.hpp>
#include <utils/JPtr.hpp>
#include <utils/Utils.hpp>


using namespace joint::devkit;
using namespace joint_java;
using namespace joint_java::binding;

JOINT_DEVKIT_LOGGER("Joint.Java.JNI")

JNIEXPORT void JNICALL Java_org_joint_JointObject_finalize(JNIEnv* env, jobject obj)
{
	JNI_WRAP_CPP_BEGIN

	JavaVM* jvm = NULL;
	JNI_CALL( env->GetJavaVM(&jvm) );

	printf("JNI FINALIZE\n");
	JLocalClassPtr cls(env, JAVA_CALL(env->GetObjectClass(obj)));
	jfieldID handle_id = JAVA_CALL(env->GetFieldID(cls, "handle", "J"));
	jlong handle_long = JAVA_CALL(env->GetLongField(obj, handle_id));
	Joint_ObjectHandle handle = (Joint_ObjectHandle)handle_long;
	printf("JNI FINALIZE handle: %p\n", handle);
	Joint_DecRefObject(handle);

	JNI_WRAP_CPP_END_VOID()
}

JNIEXPORT jobject JNICALL Java_org_joint_JointObject_invokeMethod(JNIEnv* env, jobject obj, jint methodId, jobjectArray params)
{
	JNI_WRAP_CPP_BEGIN

	JavaVM* jvm = NULL;
	JNI_CALL( env->GetJavaVM(&jvm) );

	JLocalClassPtr cls(env, JAVA_CALL(env->GetObjectClass(obj)));
	jfieldID handle_id = JAVA_CALL(env->GetFieldID(cls, "handle", "J"));
	jlong handle_long = JAVA_CALL(env->GetLongField(obj, handle_id));
	Joint_ObjectHandle handle = (Joint_ObjectHandle)handle_long;

	printf("JNI invokeMethod, handle: %p, methodId: %d\n", handle, (int)methodId);

	std::vector<Joint_Parameter> native_params;
	native_params.resize(JAVA_CALL(env->GetArrayLength(params)));
	for (size_t i = 0; i < native_params.size(); ++i)
	{
		JLocalObjPtr p(env, JAVA_CALL(env->GetObjectArrayElement(params, i)));
		JLocalClassPtr int_cls(env, JAVA_CALL(env->FindClass("java/lang/Integer")));
		jmethodID intValue_id = JAVA_CALL(env->GetMethodID(int_cls, "intValue", "()I"));
		native_params[i].value.i32 = JAVA_CALL(env->CallIntMethod(p.Get(), intValue_id));
		native_params[i].type.id = JOINT_TYPE_I32;
	}

	Joint_Type ret_type;
	ret_type.id = JOINT_TYPE_I32;
	Joint_RetValue ret_value;
	Joint_Error ret = Joint_InvokeMethod(handle, methodId, native_params.data(), native_params.size(), ret_type, &ret_value);
	JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);

	JLocalClassPtr int_cls(env, JAVA_CALL(env->FindClass("java/lang/Integer")));
	jmethodID int_ctor_id = JAVA_CALL(env->GetMethodID(int_cls, "<init>", "(I)V"));
	jobject result = JAVA_CALL(env->NewObject(int_cls, int_ctor_id, ret_value.result.value.i32));

	JNI_WRAP_CPP_END(result, NULL)
}


JNIEXPORT jobject JNICALL Java_org_joint_ModuleContext_register(JNIEnv* env, jobject moduleContext, jobject accessor)
{
	JNI_WRAP_CPP_BEGIN

	JavaVM* jvm = NULL;
	JNI_CALL( env->GetJavaVM(&jvm) );

	JLocalClassPtr cls(env, JAVA_CALL(env->GetObjectClass(moduleContext)));
	jfieldID handle_id = JAVA_CALL(env->GetFieldID(cls, "handle", "J"));
	jlong module_handle_long = JAVA_CALL(env->GetLongField(moduleContext, handle_id));

	std::unique_ptr<Object> o(new Object(JGlobalObjPtr(jvm, accessor)));
	Joint_ObjectHandle handle = JOINT_NULL_HANDLE;
	Joint_ModuleHandle module = (Joint_ModuleHandle)module_handle_long;
	Joint_Error ret = Joint_CreateObject(module, o.get(), &handle);
	JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
	o.release();
	auto sg = ScopeExit([&]{ Joint_DecRefObject(handle); });

	JLocalClassPtr obj_cls(jvm, JAVA_CALL(env->FindClass("org/joint/JointObject")));
	JOINT_CHECK(obj_cls, StringBuilder() % "Class org/joint/JointObject not found");
	jmethodID obj_ctor_id = JAVA_CALL(env->GetMethodID(obj_cls, "<init>", "(J)V"));

	// TODO: Use JLocalObjPtr?
	jobject obj = JAVA_CALL(env->NewObject(obj_cls, obj_ctor_id, (jlong)handle));
	sg.Cancel();

	JNI_WRAP_CPP_END(obj, NULL)
}


JNIEXPORT jlong JNICALL Java_org_joint_InterfaceDescriptor_initNative(JNIEnv* env, jobject self)
{
	JNI_WRAP_CPP_BEGIN
	env->NewLocalRef(self); // one new ref for JLocalObjPtr
	auto result = new JavaInterfaceDescriptor(JLocalObjPtr(env, self), JavaBindingInfo());
	JNI_WRAP_CPP_END(reinterpret_cast<jlong>(result), 0)
}


JNIEXPORT void JNICALL Java_org_joint_InterfaceDescriptor_deinitNative(JNIEnv* env, jclass cls, jlong native)
{
	JNI_WRAP_CPP_BEGIN
	delete reinterpret_cast<JavaInterfaceDescriptor*>(native);
	JNI_WRAP_CPP_END_VOID()
}
