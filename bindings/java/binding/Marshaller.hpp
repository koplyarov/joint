#ifndef BINDING_MARSHALLER_HPP
#define BINDING_MARSHALLER_HPP


#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/marshalling/ValueMarshaller.hpp>
#include <joint/devkit/util/StackStorage.hpp>

#include <memory>
#include <string.h>

#include <binding/JointJavaContext.hpp>
#include <utils/Utils.hpp>


namespace joint {
namespace java
{

    class ParamsAllocator
    {
        using JStringData = joint::java::JStringData<devkit::DefaultLifoAllocator>;

    private:
        devkit::DefaultLifoAllocator                 _alloc;
        devkit::StackStorage<JointCore_Value, 64>    _members;
        devkit::StackStorage<JStringData, 64>        _strParams;

    public:
        const char* AllocateUtf8(JNIEnv* env, const jstring& value)
        { return _strParams.MakeSingle(_alloc, JStringWeakRef(env, value)).GetData(); }

        JointCore_Value* AllocateMembers(size_t count)
        { return _members.Make(count); }
    };


    class RetValueAllocator
    {
    public:
        const char* AllocateUtf8(JNIEnv* env, const jstring& value)
        {
            devkit::DefaultLifoAllocator alloc;
            auto str_data = GetJStringData(alloc, JStringWeakRef(env, value));
            std::unique_ptr<char[]> result_str(new char[strlen(str_data.GetData()) + 1]);
            strcpy(result_str.get(), str_data.GetData());
            return result_str.release();
        }

        JointCore_Value* AllocateMembers(size_t count)
        { return new JointCore_Value[count]; }
    };


    class JavaMarshaller
    {
        JOINT_DEVKIT_LOGGER("Joint.Java.JavaMarshaller")

    public:
        using JavaVariant = jvalue;

    private:
        JNIEnv*    env;

    public:
        JavaMarshaller(JNIEnv* env)
            : env(env)
        { }

        JavaVariant FromJointBool(JointCore_Bool val) const   { jvalue result; result.z = val; return result; }
        JavaVariant FromJointU8(uint8_t val) const        { jvalue result; result.b = val; return result; }
        JavaVariant FromJointI8(int8_t val) const         { jvalue result; result.b = val; return result; }
        JavaVariant FromJointU16(uint16_t val) const      { jvalue result; result.s = val; return result; }
        JavaVariant FromJointI16(int16_t val) const       { jvalue result; result.s = val; return result; }
        JavaVariant FromJointU32(uint32_t val) const      { jvalue result; result.i = val; return result; }
        JavaVariant FromJointI32(int32_t val) const       { jvalue result; result.i = val; return result; }
        JavaVariant FromJointU64(uint64_t val) const      { jvalue result; result.j = val; return result; }
        JavaVariant FromJointI64(int64_t val) const       { jvalue result; result.j = val; return result; }
        JavaVariant FromJointF32(float val) const         { jvalue result; result.f = val; return result; }
        JavaVariant FromJointF64(double val) const        { jvalue result; result.d = val; return result; }

        JavaVariant FromJointUtf8(const char* val) const
        {
            jvalue result;
            result.l = reinterpret_cast<jobject>(JAVA_CALL(env->NewStringUTF(val)));
            return result;
        }

        JavaVariant FromJointEnum(int32_t val, const JavaBindingInfo::EnumUserData& enumType) const
        {
            jvalue result;
            result.l = JAVA_CALL(env->CallStaticObjectMethod(enumType.Cls.Get(), enumType.FromIntId, (jint)val));
            return result;
        }

        JavaVariant FromJointObj(devkit::ValueDirection dir, JointCore_ObjectAccessor val, const JavaBindingInfo::ObjectUserData& proxyType, JointCore_InterfaceChecksum checksum) const
        {
            jvalue result;
            if (JOINT_CORE_IS_NULL(val))
            {
                result.l = NULL;
                return result;
            }

            if (dir == devkit::ValueDirection::Parameter)
                JOINT_CORE_INCREF_ACCESSOR(val);

            auto sg(devkit::ScopeExit([&]{
                if (dir == devkit::ValueDirection::Parameter)
                    JOINT_CORE_DECREF_ACCESSOR(val);
            }));

            auto joint_obj = JointJavaContext::JointObject::Make(env, val);

            result.l = JAVA_CALL(env->NewObject(proxyType.Cls.Get(), proxyType.CtorId, joint_obj.Get()));
            sg.Cancel();
            return result;
        }

        JavaVariant FromJointArray(devkit::ValueDirection dir, JointCore_ArrayHandle val, const JavaBindingInfo::ArrayUserData& elementTypeDesc) const
        {
            if (dir == devkit::ValueDirection::Parameter)
                Joint_IncRefArray(val);

            JObjLocalRef array = JointJavaContext::Array::Make(env, elementTypeDesc.ElementType.Temp(env), val);

            auto sg(devkit::ScopeExit([&]{
                if (dir == devkit::ValueDirection::Parameter)
                    Joint_DecRefArray(val);
            }));

            sg.Cancel();

            jvalue result;
            result.l = array.Release();
            return result;
        }

        class ParamsArray
        {
        private:
            std::vector<jvalue>    _params;

        public:
            ParamsArray(size_t count)
                : _params(count)
            { }

            void Set(size_t index, JavaVariant param)
            { _params[index] = param; }

            const std::vector<jvalue>& GetVector() const { return _params; }
        };

        ParamsArray MakeParamsArray(size_t size) const { return ParamsArray(size); }

        JavaVariant MakeStruct(const ParamsArray& params, const JavaBindingInfo::StructUserData& structType) const
        {
            jvalue result;
            result.l = JAVA_CALL(env->NewObjectA(structType.Cls.Get(), structType.CtorId, params.GetVector().data()));
            return result;
        }

        JointCore_Bool ToJointBool(jvalue val) const  { return val.z; }
        uint8_t        ToJointU8(jvalue val) const    { return val.b; }
        int8_t         ToJointI8(jvalue val) const    { return val.b; }
        uint16_t       ToJointU16(jvalue val) const   { return val.s; }
        int16_t        ToJointI16(jvalue val) const   { return val.s; }
        uint32_t       ToJointU32(jvalue val) const   { return val.i; }
        int32_t        ToJointI32(jvalue val) const   { return val.i; }
        uint64_t       ToJointU64(jvalue val) const   { return val.j; }
        int64_t        ToJointI64(jvalue val) const   { return val.j; }
        float          ToJointF32(jvalue val) const   { return val.f; }
        double         ToJointF64(jvalue val) const   { return val.d; }

        int32_t ToJointEnum(jvalue val, const JavaBindingInfo::EnumUserData& enumType) const
        {
            jfieldID value_id = JAVA_CALL(env->GetFieldID(enumType.Cls.Get(), "value", "I"));
            return  JAVA_CALL(env->GetIntField(val.l, value_id));
        }

        JointCore_ObjectAccessor ToJointObj(devkit::ValueDirection dir, jvalue val, const JavaBindingInfo::ObjectUserData& objType) const
        {
            JointCore_ObjectAccessor result;
            if (!val.l)
            {
                JOINT_CORE_INIT_ACCESSOR(result);
                return result;
            }

            auto cls = JClassLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectClass(val.l)));
            jfieldID obj_id = JAVA_CALL(env->GetFieldID(cls.Get(), "_obj", "Lorg/joint/JointObject;"));

            auto raw_joint_obj = JObjLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(val.l, obj_id)));
            JointJavaContext::JointObject joint_obj(raw_joint_obj);

            result = joint_obj.GetAccessor();
            if (dir == devkit::ValueDirection::Return)
                JOINT_CORE_INCREF_ACCESSOR(result);
            return result;
        }

        JointCore_ArrayHandle ToJointArray(devkit::ValueDirection dir, jvalue val, const JavaBindingInfo::ArrayUserData& objType) const
        {
            auto handle = JointJavaContext::Array(JObjWeakRef(env, val.l)).GetHandle();

            if (dir == devkit::ValueDirection::Return)
                Joint_IncRefArray(handle);
            return handle;
        }

        template < typename Allocator_ >
        const char* ToJointUtf8(jvalue val, Allocator_& alloc) const
        { return alloc.AllocateUtf8(env, reinterpret_cast<jstring>(val.l)); }

        template < typename MemberInfo_ >
        jvalue GetStructMember(jvalue val, size_t i, const MemberInfo_& memberInfo, const JavaBindingInfo::StructUserData& structType) const
        {
            jvalue res;
            switch (memberInfo.GetType().GetJointType().id)
            {
            case JOINT_CORE_TYPE_BOOL:
                res.z = env->GetBooleanField(val.l, memberInfo.GetMemberId().Id);
                break;
            case JOINT_CORE_TYPE_U8:
            case JOINT_CORE_TYPE_I8:
                res.b = env->GetByteField(val.l, memberInfo.GetMemberId().Id);
                break;
            case JOINT_CORE_TYPE_U16:
            case JOINT_CORE_TYPE_I16:
                res.s = env->GetShortField(val.l, memberInfo.GetMemberId().Id);
                break;
            case JOINT_CORE_TYPE_U32:
            case JOINT_CORE_TYPE_I32:
                res.i = env->GetIntField(val.l, memberInfo.GetMemberId().Id);
                break;
            case JOINT_CORE_TYPE_U64:
            case JOINT_CORE_TYPE_I64:
                res.j = env->GetLongField(val.l, memberInfo.GetMemberId().Id);
                break;
            case JOINT_CORE_TYPE_F32:
                res.f = env->GetFloatField(val.l, memberInfo.GetMemberId().Id);
                break;
            case JOINT_CORE_TYPE_F64:
                res.d = env->GetDoubleField(val.l, memberInfo.GetMemberId().Id);
                break;
            case JOINT_CORE_TYPE_UTF8:
            case JOINT_CORE_TYPE_ENUM:
            case JOINT_CORE_TYPE_OBJ:
            case JOINT_CORE_TYPE_STRUCT:
                res.l = env->GetObjectField(val.l, memberInfo.GetMemberId().Id);
                break;
            default:
                JOINT_DEVKIT_THROW(JOINT_CORE_ERROR_NOT_IMPLEMENTED);
            }

            return res;
        }
    };

}}

#endif
