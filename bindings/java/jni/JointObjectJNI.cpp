#include <JointJNI.h>
#include <binding/Boxing.hpp>
#include <binding/JavaBindingInfo.hpp>
#include <binding/Marshaller.hpp>


using namespace joint::devkit;
using namespace joint::java;

static const char* LoggerName = "Joint.Java.JNI";
JOINT_DEVKIT_LOGGER(LoggerName)


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


namespace
{
	template < typename T>
	JObjLocalRef WrapResult(JNIEnv* env, Joint_Error ret, const T& retType, Joint_RetValue ret_value)
	{
		Joint_Type ret_type = retType.GetJointType();
		if (ret == JOINT_ERROR_NONE)
		{
			auto sg(ScopeExit([&]{
				Joint_Error ret = ret_value.releaseValue(ret_type, ret_value.result.value);
				if (ret != JOINT_ERROR_NONE)
					GetLogger().Error() << "Joint_RetValue::releaseValue failed: " << ret;
			}));

			if (ret_type.id == JOINT_TYPE_VOID)
				return JObjLocalRef();

			jvalue v = ValueMarshaller::FromJoint<jvalue>(ValueDirection::Return, retType, ret_value.result.value, JavaMarshaller(env));
			return Boxing(env).Box(retType.GetJointType(), v);
		}
		else if (ret == JOINT_ERROR_EXCEPTION)
		{
			auto JointException_cls = JClassLocalRef::StealLocal(env, JAVA_CALL(env->FindClass("Lorg/joint/JointException;")));
			jmethodID JointException_ctor_id = JAVA_CALL(env->GetMethodID(JointException_cls.Get(), "<init>", "(J)V"));
			std::unique_ptr<ExceptionInfo> ex_info(new ExceptionInfo(ExceptionInfo::FromJointException(ret_value.result.ex)));
			auto ex = JThrowableLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(JointException_cls.Get(), JointException_ctor_id, ex_info.get())));
			ex_info.release();
			env->Throw(ex.Get());
			return JObjLocalRef();
		}
		else
			JOINT_THROW((std::string("Joint_InvokeMethod failed: ") + Joint_ErrorToString(ret)).c_str());
	}

	template < typename Marshaller_, typename TypeDesc_, typename ParamsAllocator_ >
	Joint_Parameter UnwrapParameter(JNIEnv* env, const Marshaller_& m, const TypeDesc_& t, ParamsAllocator_& alloc, JObjWeakRef p)
	{
		Joint_Value v = ValueMarshaller::ToJoint(ValueDirection::Parameter, t, Boxing(env).Unbox(t.GetJointType(), p), m, alloc);
		return Joint_Parameter{v, t.GetJointType()};
	}
}


#define DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(ParamsCount_, ...) \
	JNI_WRAP_CPP_BEGIN \
	\
	auto obj = reinterpret_cast<Joint_ObjectHandle>(handle); \
	auto ifc_desc = reinterpret_cast<JavaInterfaceDescriptor*>(nativeInterfaceDescriptor); \
	JOINT_ASSERT(obj && ifc_desc); \
	\
	const auto& method_desc = ifc_desc->GetMethod(methodId); \
	\
	__VA_ARGS__ \
	\
	Joint_RetValue ret_value; \
	Joint_Error ret = Joint_InvokeMethod(obj, methodId, params, ParamsCount_, method_desc.GetRetType().GetJointType(), &ret_value); \
	JObjLocalRef result = WrapResult(env, ret, method_desc.GetRetType(), ret_value); \
	\
	JNI_WRAP_CPP_END(result.Release(), nullptr)

#define DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(I_) \
	params[I_] = UnwrapParameter(env, m, method_desc.GetParamType(I_), alloc, JObjWeakRef(env, p##I_));

JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doInvokeMethod0(JNIEnv* env, jclass cls, jlong handle, jlong nativeInterfaceDescriptor, jint methodId)
{
	DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(0,
		Joint_Parameter* params = nullptr;
	)
}


JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doInvokeMethod1(JNIEnv* env, jclass cls, jlong handle, jlong nativeInterfaceDescriptor, jint methodId,
	jobject p0)
{
	DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(1,
		ParamsAllocator alloc;
		JavaMarshaller m(env);
		Joint_Parameter params[1];
		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(0)
	)
}

JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doInvokeMethod2(JNIEnv* env, jclass cls, jlong handle, jlong nativeInterfaceDescriptor, jint methodId,
	jobject p0, jobject p1)
{
	DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(2,
		ParamsAllocator alloc;
		JavaMarshaller m(env);
		Joint_Parameter params[2];
		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(0)
		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(1)
	)
}

JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doInvokeMethod3(JNIEnv* env, jclass cls, jlong handle, jlong nativeInterfaceDescriptor, jint methodId,
	jobject p0, jobject p1, jobject p2)
{
	DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(3,
		ParamsAllocator alloc;
		JavaMarshaller m(env);
		Joint_Parameter params[3];
		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(0)
		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(1)
		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(2)
	)
}


JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doInvokeMethod4(JNIEnv* env, jclass cls, jlong handle, jlong nativeInterfaceDescriptor, jint methodId,
	jobject p0, jobject p1, jobject p2, jobject p3)
{
	DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(4,
		ParamsAllocator alloc;
		JavaMarshaller m(env);
		Joint_Parameter params[4];
		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(0)
		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(1)
		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(2)
		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(3)
	)
}

JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doInvokeMethod5(JNIEnv* env, jclass cls, jlong handle, jlong nativeInterfaceDescriptor, jint methodId, jint paramsCount,
	jobject p0, jobject p1, jobject p2, jobject p3,
	jobjectArray allP)
{
	DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(paramsCount,
		ParamsAllocator alloc;
		JavaMarshaller m(env);

		StackStorage<Joint_Parameter, 64> params_storage;
		Joint_Parameter* params = params_storage.Make(paramsCount);

		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(0)
		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(1)
		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(2)
		DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(3)

		for (int i = 4; i < paramsCount; ++i)
		{
			const auto& t = method_desc.GetParamType(i);

			auto p = JObjLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectArrayElement(allP, i)));

			Joint_Value v = ValueMarshaller::ToJoint(ValueDirection::Parameter, t, Boxing(env).Unbox(t.GetJointType(), p), JavaMarshaller(env), alloc);
			params[i] = Joint_Parameter{v, t.GetJointType()};
		}
	)
}



