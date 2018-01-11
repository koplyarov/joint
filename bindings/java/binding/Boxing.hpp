#ifndef BINDING_BOXING_HPP
#define BINDING_BOXING_HPP


#include <binding/JointJavaContext.hpp>
#include <utils/JPtr.hpp>
#include <utils/Utils.hpp>


namespace joint {
namespace java
{

    class Boxing
    {
        JOINT_DEVKIT_LOGGER("Joint.Java.Boxing");

    private:
        JNIEnv*    env;

    public:
        Boxing(JNIEnv* env)
            : env(env)
        { }

        jvalue Unbox(JointCore_Type type, const JObjWeakRef& boxed) const
        {
            jvalue result;

            switch (type.id)
            {
            case JOINT_CORE_TYPE_BOOL:
                result.z = UnboxBoolean(boxed);
                break;
            case JOINT_CORE_TYPE_U8:
            case JOINT_CORE_TYPE_I8:
                result.b = UnboxByte(boxed);
                break;
            case JOINT_CORE_TYPE_U16:
            case JOINT_CORE_TYPE_I16:
                result.s = UnboxShort(boxed);
                break;
            case JOINT_CORE_TYPE_U32:
            case JOINT_CORE_TYPE_I32:
                result.i = UnboxInteger(boxed);
                break;
            case JOINT_CORE_TYPE_U64:
            case JOINT_CORE_TYPE_I64:
                result.j = UnboxLong(boxed);
                break;
            case JOINT_CORE_TYPE_F32:
                result.f = UnboxFloat(boxed);
                break;
            case JOINT_CORE_TYPE_F64:
                result.d = UnboxDouble(boxed);
                break;
            case JOINT_CORE_TYPE_UTF8:
            case JOINT_CORE_TYPE_ENUM:
            case JOINT_CORE_TYPE_OBJ:
            case JOINT_CORE_TYPE_STRUCT:
                result.l = boxed.Get();
                break;
            default:
                JOINT_DEVKIT_THROW(JOINT_CORE_ERROR_NOT_IMPLEMENTED);
            }

            return result;
        }

        JObjLocalRef Box(JointCore_Type type, jvalue unboxed) const
        {
            switch (type.id)
            {
            case JOINT_CORE_TYPE_BOOL:
                return BoxBoolean(unboxed.z);
            case JOINT_CORE_TYPE_U8:
            case JOINT_CORE_TYPE_I8:
                return BoxByte(unboxed.b);
            case JOINT_CORE_TYPE_U16:
            case JOINT_CORE_TYPE_I16:
                return BoxShort(unboxed.s);
            case JOINT_CORE_TYPE_U32:
            case JOINT_CORE_TYPE_I32:
                return BoxInteger(unboxed.i);
            case JOINT_CORE_TYPE_U64:
            case JOINT_CORE_TYPE_I64:
                return BoxLong(unboxed.j);
            case JOINT_CORE_TYPE_F32:
                return BoxFloat(unboxed.f);
            case JOINT_CORE_TYPE_F64:
                return BoxDouble(unboxed.d);
            case JOINT_CORE_TYPE_UTF8:
            case JOINT_CORE_TYPE_ENUM:
            case JOINT_CORE_TYPE_OBJ:
            case JOINT_CORE_TYPE_STRUCT:
                return JObjLocalRef::StealLocal(env, unboxed.l);
            default:
                JOINT_DEVKIT_THROW(JOINT_CORE_ERROR_NOT_IMPLEMENTED);
            }
        }

    private:
#define DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(TypeName_, CppType_, JniType_, FieldAccess_) \
            CppType_ Unbox##TypeName_(const JObjWeakRef& val) const \
            { \
                const auto& jctx = JointJavaContext::ConstInstance(); \
                return static_cast<CppType_>(JAVA_CALL(env->Get##FieldAccess_(val.Get(), jctx.TypeName_##_value))); \
            } \
            JObjLocalRef Box##TypeName_(CppType_ val) const \
            { \
                const auto& jctx = JointJavaContext::ConstInstance(); \
                auto res = JObjLocalRef::StealLocal(env, env->AllocObject(jctx.TypeName_##_cls.Get())); \
                if (res) \
                    env->Set##FieldAccess_(res.Get(), jctx.TypeName_##_value, val); \
                if (env->ExceptionCheck()) \
                    ThrowExceptionFromJava(env, JOINT_DEVKIT_SOURCE_LOCATION); \
                return res; \
            }

        DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Boolean, JointCore_Bool, jboolean, BooleanField)
        DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Byte,    int8_t,         jbyte,    ByteField)
        DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Short,   int16_t,        jshort,   ShortField)
        DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Integer, int32_t,        jint,     IntField)
        DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Long,    int64_t,        jlong,    LongField)
        DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Float,   float,          jfloat,   FloatField)
        DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Double,  double,         jdouble,  DoubleField)

#undef DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING

    };

}}

#endif
