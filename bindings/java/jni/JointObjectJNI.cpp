#include <joint/devkit/util/ScopeExit.hpp>

#include <JointJNI.h>
#include <binding/Boxing.hpp>
#include <binding/JavaBindingInfo.hpp>
#include <binding/Marshaller.hpp>
#include <jni/JNIHelpers.hpp>


using namespace joint::devkit;
using namespace joint::java;

static const char* LoggerName = "Joint.Java.JNI";
JOINT_DEVKIT_LOGGER(LoggerName)


JNIEXPORT void JNICALL Java_org_joint_JointObject_releaseObject(JNIEnv* env, jclass cls, jlong accessorVTableLong, jlong accessorInstanceLong)
{
    JNI_WRAP_CPP_BEGIN

    JOINT_CORE_DECREF_ACCESSOR(ObjectAccessorFromJLongs(accessorVTableLong, accessorInstanceLong));

    JNI_WRAP_CPP_END_VOID()
}


JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doCast(JNIEnv* env, jclass cls, jlong accessorVTableLong, jlong accessorInstanceLong, jstring interfaceId, jint interfaceChecksum)
{
    JNI_WRAP_CPP_BEGIN

    JointCore_ObjectAccessor accessor = ObjectAccessorFromJLongs(accessorVTableLong, accessorInstanceLong);

    DefaultLifoAllocator alloc;

    JointCore_ObjectAccessor new_accessor;
    JointCore_Error ret = accessor.VTable->CastObject(accessor.Instance, GetJStringData(alloc, JStringWeakRef(env, interfaceId)).GetData(), interfaceChecksum, &new_accessor);
    JOINT_DEVKIT_CHECK(ret == JOINT_CORE_ERROR_NONE || JOINT_CORE_ERROR_CAST_FAILED, ret);

    JObjLocalRef result;
    if (ret == JOINT_CORE_ERROR_NONE)
        result = JointJavaContext::JointObject::Make(env, new_accessor);

    JNI_WRAP_CPP_END(result.Release(), nullptr)
}


namespace
{
    template < typename T>
    JObjLocalRef WrapResult(JNIEnv* env, JointCore_Error ret, const T& retType, JointCore_RetValue ret_value)
    {
        JointCore_Type ret_type = retType.GetJointType();
        if (ret == JOINT_CORE_ERROR_NONE)
        {
            auto sg(ScopeExit([&]{
                JointCore_Error ret = ret_value.releaseValue(ret_type, ret_value.result.value);
                if (ret != JOINT_CORE_ERROR_NONE)
                    GetLogger().Error() << "JointCore_RetValue::releaseValue failed: " << ret;
            }));

            if (ret_type.id == JOINT_CORE_TYPE_VOID)
                return JObjLocalRef();

            jvalue v = ValueMarshaller::FromJoint<jvalue>(ValueDirection::Return, retType, ret_value.result.value, JavaMarshaller(env));
            return Boxing(env).Box(retType.GetJointType(), v);
        }
        else if (ret == JOINT_CORE_ERROR_EXCEPTION)
        {
            auto sg = ScopeExit([&]{ JointCore_Exception_DecRef(ret_value.result.ex); });
            auto ex = JointJavaContext::JointException::Make(env, ret_value.result.ex);
            sg.Cancel();
            env->Throw(ex.Get());
            return JObjLocalRef();
        }
        else
            JOINT_DEVKIT_THROW((std::string("Joint_InvokeMethod failed: ") + JointCore_ErrorToString(ret)).c_str());
    }

    template < typename Marshaller_, typename TypeDesc_, typename ParamsAllocator_ >
    JointCore_Parameter UnwrapParameter(JNIEnv* env, const Marshaller_& m, const TypeDesc_& t, ParamsAllocator_& alloc, JObjWeakRef p)
    {
        JointCore_Value v = ValueMarshaller::ToJoint(ValueDirection::Parameter, t, Boxing(env).Unbox(t.GetJointType(), p), m, alloc);
        return JointCore_Parameter{v};
    }
}


#define DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(ParamsCount_, ...) \
    JNI_WRAP_CPP_BEGIN \
    \
    auto obj = ObjectAccessorFromJLongs(accessorVTableLong, accessorInstanceLong); \
    auto ifc_desc = reinterpret_cast<JavaInterfaceDescriptor*>(nativeInterfaceDescriptor); \
    JOINT_DEVKIT_ASSERT(!JOINT_CORE_IS_NULL(obj) && ifc_desc); \
    \
    const auto& method_desc = ifc_desc->GetMethod(methodId); \
    \
    __VA_ARGS__ \
    \
    JointCore_RetValue ret_value; \
    JointCore_Error ret = obj.VTable->InvokeMethod(obj.Instance, methodId, params, ParamsCount_, &ret_value); \
    JObjLocalRef result = WrapResult(env, ret, method_desc.GetRetType(), ret_value); \
    \
    JNI_WRAP_CPP_END(result.Release(), nullptr)

#define DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(I_) \
    params[I_] = UnwrapParameter(env, m, method_desc.GetParamType(I_), alloc, JObjWeakRef(env, p##I_));

JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doInvokeMethod0(JNIEnv* env, jclass cls, jlong accessorVTableLong, jlong accessorInstanceLong, jlong nativeInterfaceDescriptor, jint methodId)
{
    DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(0,
        JointCore_Parameter* params = nullptr;
    )
}


JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doInvokeMethod1(JNIEnv* env, jclass cls, jlong accessorVTableLong, jlong accessorInstanceLong, jlong nativeInterfaceDescriptor, jint methodId,
    jobject p0)
{
    DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(1,
        ParamsAllocator alloc;
        JavaMarshaller m(env);
        JointCore_Parameter params[1];
        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(0)
    )
}

JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doInvokeMethod2(JNIEnv* env, jclass cls, jlong accessorVTableLong, jlong accessorInstanceLong, jlong nativeInterfaceDescriptor, jint methodId,
    jobject p0, jobject p1)
{
    DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(2,
        ParamsAllocator alloc;
        JavaMarshaller m(env);
        JointCore_Parameter params[2];
        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(0)
        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(1)
    )
}

JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doInvokeMethod3(JNIEnv* env, jclass cls, jlong accessorVTableLong, jlong accessorInstanceLong, jlong nativeInterfaceDescriptor, jint methodId,
    jobject p0, jobject p1, jobject p2)
{
    DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(3,
        ParamsAllocator alloc;
        JavaMarshaller m(env);
        JointCore_Parameter params[3];
        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(0)
        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(1)
        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(2)
    )
}


JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doInvokeMethod4(JNIEnv* env, jclass cls, jlong accessorVTableLong, jlong accessorInstanceLong, jlong nativeInterfaceDescriptor, jint methodId,
    jobject p0, jobject p1, jobject p2, jobject p3)
{
    DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(4,
        ParamsAllocator alloc;
        JavaMarshaller m(env);
        JointCore_Parameter params[4];
        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(0)
        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(1)
        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(2)
        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(3)
    )
}

JNIEXPORT jobject JNICALL Java_org_joint_JointObject_doInvokeMethod5(JNIEnv* env, jclass cls, jlong accessorVTableLong, jlong accessorInstanceLong, jlong nativeInterfaceDescriptor, jint methodId, jint paramsCount,
    jobject p0, jobject p1, jobject p2, jobject p3,
    jobjectArray allP)
{
    DETAIL_JOINT_JAVA_DO_INVOKE_METHOD(paramsCount,
        ParamsAllocator alloc;
        JavaMarshaller m(env);

        StackStorage<JointCore_Parameter, 64> params_storage;
        JointCore_Parameter* params = params_storage.Make(paramsCount);

        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(0)
        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(1)
        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(2)
        DETAIL_JOINT_JAVA_UNWRAP_PARAMETER(3)

        for (int i = 4; i < paramsCount; ++i)
        {
            const auto& t = method_desc.GetParamType(i);

            auto p = JObjLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectArrayElement(allP, i)));

            JointCore_Value v = ValueMarshaller::ToJoint(ValueDirection::Parameter, t, Boxing(env).Unbox(t.GetJointType(), p), JavaMarshaller(env), alloc);
            params[i] = JointCore_Parameter{v};
        }
    )
}



