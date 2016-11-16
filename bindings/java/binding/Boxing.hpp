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

		jvalue Unbox(Joint_Type type, const JObjWeakRef& boxed) const
		{
			jvalue result;

			switch (type.id)
			{
			case JOINT_TYPE_BOOL:
				result.z = UnboxBoolean(boxed);
				break;
			case JOINT_TYPE_U8:
			case JOINT_TYPE_I8:
				result.b = UnboxByte(boxed);
				break;
			case JOINT_TYPE_U16:
			case JOINT_TYPE_I16:
				result.s = UnboxShort(boxed);
				break;
			case JOINT_TYPE_U32:
			case JOINT_TYPE_I32:
				result.i = UnboxInteger(boxed);
				break;
			case JOINT_TYPE_U64:
			case JOINT_TYPE_I64:
				result.j = UnboxLong(boxed);
				break;
			case JOINT_TYPE_F32:
				result.f = UnboxFloat(boxed);
				break;
			case JOINT_TYPE_F64:
				result.d = UnboxDouble(boxed);
				break;
			case JOINT_TYPE_UTF8:
			case JOINT_TYPE_ENUM:
			case JOINT_TYPE_OBJ:
			case JOINT_TYPE_STRUCT:
				result.l = boxed.Get();
				break;
			default:
				JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED);
			}

			return result;
		}

		JObjLocalRef Box(Joint_Type type, jvalue unboxed) const
		{
			switch (type.id)
			{
			case JOINT_TYPE_BOOL:
				return BoxBoolean(unboxed.z);
			case JOINT_TYPE_U8:
			case JOINT_TYPE_I8:
				return BoxByte(unboxed.b);
			case JOINT_TYPE_U16:
			case JOINT_TYPE_I16:
				return BoxShort(unboxed.s);
			case JOINT_TYPE_U32:
			case JOINT_TYPE_I32:
				return BoxInteger(unboxed.i);
			case JOINT_TYPE_U64:
			case JOINT_TYPE_I64:
				return BoxLong(unboxed.j);
			case JOINT_TYPE_F32:
				return BoxFloat(unboxed.f);
			case JOINT_TYPE_F64:
				return BoxDouble(unboxed.d);
			case JOINT_TYPE_UTF8:
			case JOINT_TYPE_ENUM:
			case JOINT_TYPE_OBJ:
			case JOINT_TYPE_STRUCT:
				return JObjLocalRef::StealLocal(env, unboxed.l);
			default:
				JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED);
			}
		}

	private:
#define DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(TypeName_, CppType_, JniType_, GetField_) \
			CppType_ Unbox##TypeName_(const JObjWeakRef& val) const \
			{ \
				const auto& jctx = JointJavaContext::ConstInstance(); \
				return static_cast<CppType_>(JAVA_CALL(env->GetField_(val.Get(), jctx.TypeName_##_value))); \
			} \
			JObjLocalRef Box##TypeName_(CppType_ val) const \
			{ \
				const auto& jctx = JointJavaContext::ConstInstance(); \
				return JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(jctx.TypeName_##_cls.Get(), jctx.TypeName_##_ctor, (JniType_)val))); \
			}

		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Boolean, Joint_Bool, jboolean, GetBooleanField)
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Byte,    int8_t,     jbyte,    GetByteField)
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Short,   int16_t,    jshort,   GetShortField)
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Integer, int32_t,    jint,     GetIntField)
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Long,    int64_t,    jlong,    GetLongField)
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Float,   float,      jfloat,   GetFloatField)
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Double,  double,     jdouble,  GetDoubleField)

#undef DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING

	};

}}

#endif
