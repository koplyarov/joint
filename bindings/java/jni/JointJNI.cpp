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

JNIEXPORT void JNICALL Java_org_joint_JointObject_releaseObject(JNIEnv* env, jclass cls, jlong handleLong)
{
	JNI_WRAP_CPP_BEGIN

	Joint_ObjectHandle handle = (Joint_ObjectHandle)handleLong;
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

		JLocalObjPtr p(env, JAVA_CALL(env->GetObjectArrayElement(jparams, i)));
		Joint_Value v = ValueMarshaller::ToJoint(ValueDirection::Parameter, t, p, JavaProxyMarshaller(env), alloc);
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

		result = ValueMarshaller::FromJoint<jobject>(ValueDirection::Return, method_desc.GetRetType(), ret_value.result.value, JavaProxyMarshaller(env));
	}
	else
		JOINT_THROW((std::string("Joint_InvokeMethod failed: ") + Joint_ErrorToString(ret)).c_str());

	JNI_WRAP_CPP_END(result, nullptr)
}


JNIEXPORT jlong JNICALL Java_org_joint_ModuleContext_doRegister(JNIEnv* env, jclass cls, jlong moduleHandleLong, jobject accessor)
{
	JNI_WRAP_CPP_BEGIN

	JavaVM* jvm = nullptr;
	JNI_CALL( env->GetJavaVM(&jvm) );

	std::unique_ptr<Object> o(new Object(JGlobalObjPtr(jvm, accessor)));
	Joint_ObjectHandle handle = JOINT_NULL_HANDLE;
	Joint_ModuleHandle module = (Joint_ModuleHandle)moduleHandleLong;
	Joint_Error ret = Joint_CreateObject(module, o.get(), &handle);
	JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
	o.release();

	JNI_WRAP_CPP_END(reinterpret_cast<jlong>(handle), 0)
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
