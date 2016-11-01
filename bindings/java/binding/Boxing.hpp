#ifndef BINDING_BOXING_HPP
#define BINDING_BOXING_HPP


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

		jvalue Unbox(Joint_Type type, const JObjRef& boxed) const
		{
			jvalue result;

			switch (type.id)
			{
			case JOINT_TYPE_BOOL:
				result.z = UnboxBool(boxed);
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
				result.i = UnboxInt(boxed);
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
				result.l = boxed;
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
				return BoxBool(unboxed.z);
			case JOINT_TYPE_U8:
			case JOINT_TYPE_I8:
				return BoxByte(unboxed.b);
			case JOINT_TYPE_U16:
			case JOINT_TYPE_I16:
				return BoxShort(unboxed.s);
			case JOINT_TYPE_U32:
			case JOINT_TYPE_I32:
				return BoxInt(unboxed.i);
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
#define DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(TypeName_, CppType_, JniType_, CallMethod_, ClassName_, ValueGetterName_, MangledType_) \
			CppType_ Unbox##TypeName_(const JObjRef& val) const \
			{ \
				auto cls = JClassLocalRef::StealLocal(env, JAVA_CALL(env->FindClass(ClassName_))); \
				jmethodID method_id = JAVA_CALL(env->GetMethodID(cls, ValueGetterName_, "()" MangledType_)); \
				return static_cast<CppType_>(JAVA_CALL(env->CallMethod_(val, method_id))); \
			} \
			JObjLocalRef Box##TypeName_(CppType_ val) const \
			{ \
				auto cls = JClassLocalRef::StealLocal(env, JAVA_CALL(env->FindClass(ClassName_))); \
				jmethodID ctor_id = JAVA_CALL(env->GetMethodID(cls, "<init>", "(" MangledType_ ")V")); \
				return JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(cls, ctor_id, (JniType_)val))); \
			}

		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Bool,   Joint_Bool, jboolean, CallBooleanMethod, "java/lang/Boolean", "booleanValue", "Z")
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Byte,   int8_t,     jbyte,    CallByteMethod,    "java/lang/Byte",    "byteValue",    "B")
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Short,  int16_t,    jshort,   CallShortMethod,   "java/lang/Short",   "shortValue",   "S")
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Int,    int32_t,    jint,     CallIntMethod,     "java/lang/Integer", "intValue",     "I")
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Long,   int64_t,    jlong,    CallLongMethod,    "java/lang/Long",    "longValue",    "J")
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Float,  float,      jfloat,   CallFloatMethod,   "java/lang/Float",   "floatValue",   "F")
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING(Double, double,     jdouble,  CallDoubleMethod,  "java/lang/Double",  "doubleValue",  "D")

#undef DETAIL_JOINT_JAVA_SIMPLE_TYPE_BOXING

	};

}}

#endif
