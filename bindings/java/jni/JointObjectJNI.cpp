#include <JointJNI.h>
#include <binding/Boxing.hpp>
#include <binding/JavaBindingInfo.hpp>
#include <binding/Marshaller.hpp>


using namespace joint::devkit;
using namespace joint::java;

JOINT_DEVKIT_LOGGER("Joint.Java.JNI")


JNIEXPORT void JNICALL Java_org_joint_JointObject_releaseObject(JNIEnv* env, jclass cls, jlong handleLong)
{
	JNI_WRAP_CPP_BEGIN

	Joint_DecRefObject(reinterpret_cast<Joint_ObjectHandle>(handleLong));

	JNI_WRAP_CPP_END_VOID()
}


JNIEXPORT jlong JNICALL Java_org_joint_JointObject_doCast(JNIEnv* env, jclass cls, jlong handleLong, jstring interfaceId, jint interfaceChecksum)
{
	JNI_WRAP_CPP_BEGIN

	Joint_ObjectHandle handle = reinterpret_cast<Joint_ObjectHandle>(handleLong);

	Joint_ObjectHandle new_handle = JOINT_NULL_HANDLE;
	Joint_Error ret = Joint_CastObject(handle, StringDataHolder(JStringWeakRef(env, interfaceId)).GetData(), interfaceChecksum, &new_handle);
	JOINT_CHECK(ret == JOINT_ERROR_NONE || JOINT_ERROR_CAST_FAILED, ret);


	JNI_WRAP_CPP_END(ret == JOINT_ERROR_NONE ? reinterpret_cast<jlong>(new_handle) : 0, 0)
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

		auto p = JObjLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectArrayElement(jparams, i)));
		Joint_Value v = ValueMarshaller::ToJoint(ValueDirection::Parameter, t, Boxing(env).Unbox(t.GetJointType(), p), JavaMarshaller(env), alloc);
		params[i] = Joint_Parameter{v, t.GetJointType()};
	}

	Joint_Type ret_type = method_desc.GetRetType().GetJointType();
	Joint_RetValue ret_value;
	Joint_Error ret = Joint_InvokeMethod(obj, methodId, params, params_count, ret_type, &ret_value);
	JOINT_CHECK(ret == JOINT_ERROR_NONE || ret == JOINT_ERROR_EXCEPTION, (std::string("Joint_InvokeMethod failed: ") + Joint_ErrorToString(ret)).c_str());

	JObjLocalRef result;

	if (ret == JOINT_ERROR_NONE)
	{
		auto sg(ScopeExit([&]{
			Joint_Error ret = ret_value.releaseValue(ret_type, ret_value.result.value);
			if (ret != JOINT_ERROR_NONE)
				GetLogger().Error() << "Joint_RetValue::releaseValue failed: " << ret;
		}));

		if (ret_type.id == JOINT_TYPE_VOID)
			return nullptr;

		jvalue v = ValueMarshaller::FromJoint<jvalue>(ValueDirection::Return, method_desc.GetRetType(), ret_value.result.value, JavaMarshaller(env));
		result = Boxing(env).Box(method_desc.GetRetType().GetJointType(), v);
	}
	else if (ret == JOINT_ERROR_EXCEPTION)
	{
		auto JointException_cls = JClassLocalRef::StealLocal(env, JAVA_CALL(env->FindClass("Lorg/joint/JointException;")));
		jmethodID JointException_ctor_id = JAVA_CALL(env->GetMethodID(JointException_cls.Get(), "<init>", "(J)V"));
		std::unique_ptr<ExceptionInfo> ex_info(new ExceptionInfo(ExceptionInfo::FromJointException(ret_value.result.ex)));
		auto ex = JThrowableLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(JointException_cls.Get(), JointException_ctor_id, ex_info.get())));
		ex_info.release();
		env->Throw(ex.Get());
	}
	else
		JOINT_THROW((std::string("Joint_InvokeMethod failed: ") + Joint_ErrorToString(ret)).c_str());

	JNI_WRAP_CPP_END(result.Release(), nullptr)
}


