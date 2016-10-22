#include <joint/devkit/ScopeExit.hpp>
#include <joint/devkit/StackStorage.hpp>

#include <vector>

#include <JointJNI.h>
#include <binding/JavaBindingInfo.hpp>
#include <binding/Marshaller.hpp>
#include <binding/Object.hpp>
#include <utils/JPtr.hpp>
#include <utils/Utils.hpp>


using namespace joint::devkit;
using namespace joint::java;
using namespace joint::java::binding;

JOINT_DEVKIT_LOGGER("Joint.Java.JNI")

JNIEXPORT void JNICALL Java_org_joint_JointObject_finalize(JNIEnv* env, jobject obj)
{
	JNI_WRAP_CPP_BEGIN

	JavaVM* jvm = nullptr;
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

JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doInvokeMethod(JNIEnv* env, jclass cls, jlong handle, jlong nativeInterfaceDescriptor, jint methodId, jobjectArray jparams)
{
	JNI_WRAP_CPP_BEGIN

	JavaVM* jvm = nullptr;
	JNI_CALL( env->GetJavaVM(&jvm) );

	auto obj = reinterpret_cast<Joint_ObjectHandle>(handle);
	JOINT_CHECK(obj, "Invalid object");

	auto ifc_desc = reinterpret_cast<JavaInterfaceDescriptor*>(nativeInterfaceDescriptor);
	JOINT_CHECK(ifc_desc, "Invalid object");

	const auto& method_desc = ifc_desc->GetMethod(methodId);

	StackStorage<Joint_Parameter, 64> params_storage;

	auto params_count = JAVA_CALL(env->GetArrayLength(jparams));
	auto params = params_storage.Make(params_count);

	ParamsAllocator alloc;

	for (auto i = 0; i < params_count; ++i)
	{
		const auto& t = method_desc.GetParamType(i);

		jvalue p;
		switch (t.GetJointType().id)
		{
		case JOINT_TYPE_I32:
			{
				JLocalObjPtr p_obj(env, JAVA_CALL(env->GetObjectArrayElement(jparams, i)));
				JLocalClassPtr int_cls(env, JAVA_CALL(env->FindClass("java/lang/Integer")));
				jmethodID intValue_id = JAVA_CALL(env->GetMethodID(int_cls, "intValue", "()I"));
				p.i = JAVA_CALL(env->CallIntMethod(p_obj.Get(), intValue_id));
			}
			break;
		default:
			JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED);
		}

		Joint_Value v = ValueMarshaller::ToJoint(ValueDirection::Parameter, t, p, JavaMarshaller(), alloc);
		params[i] = Joint_Parameter{v, t.GetJointType()};
	}

	Joint_Type ret_type = method_desc.GetRetType().GetJointType();
	Joint_RetValue ret_value;
	Joint_Error ret = Joint_InvokeMethod(obj, methodId, params, params_count, ret_type, &ret_value);
	JOINT_CHECK(ret == JOINT_ERROR_NONE || ret == JOINT_ERROR_EXCEPTION, (std::string("Joint_InvokeMethod failed: ") + Joint_ErrorToString(ret)).c_str());

	jobject result = nullptr;

	if (ret == JOINT_ERROR_NONE)
	{
		auto sg(ScopeExit([&]{
			Joint_Error ret = ret_value.releaseValue(ret_type, ret_value.result.value);
			if (ret != JOINT_ERROR_NONE)
				GetLogger().Error() << "Joint_RetValue::releaseValue failed: " << ret;
		}));

		if (ret_type.id == JOINT_TYPE_VOID)
			return nullptr;

		jvalue jret_value = ValueMarshaller::FromJoint<jvalue>(ValueDirection::Return, method_desc.GetRetType(), ret_value.result.value, JavaMarshaller());
		switch (ret_type.id)
		{
		case JOINT_TYPE_I32:
			{
				JLocalClassPtr int_cls(env, JAVA_CALL(env->FindClass("java/lang/Integer")));
				jmethodID int_ctor_id = JAVA_CALL(env->GetMethodID(int_cls, "<init>", "(I)V"));
				result = JAVA_CALL(env->NewObject(int_cls, int_ctor_id, jret_value.i));
			}
			break;
		default:
			JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED);
		}
	}
	else
		JOINT_THROW((std::string("Joint_InvokeMethod failed: ") + Joint_ErrorToString(ret)).c_str());

	JNI_WRAP_CPP_END(result, nullptr)
}


JNIEXPORT jobject JNICALL Java_org_joint_ModuleContext_register(JNIEnv* env, jobject moduleContext, jobject accessor)
{
	JNI_WRAP_CPP_BEGIN

	JavaVM* jvm = nullptr;
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

	JNI_WRAP_CPP_END(obj, nullptr)
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
