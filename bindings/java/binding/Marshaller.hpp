#ifndef BINDING_MARSHALLER_HPP
#define BINDING_MARSHALLER_HPP


#include <joint/devkit/Logger.hpp>
#include <joint/devkit/StackStorage.hpp>
#include <joint/devkit/ValueMarshaller.hpp>

#include <memory>
#include <string.h>

#include <utils/Utils.hpp>


namespace joint {
namespace java
{

	class ParamsAllocator
	{
	private:
		devkit::StackStorage<Joint_Value, 64>        _members;
		devkit::StackStorage<StringDataHolder, 64>   _strParams;

	public:
		const char* AllocateUtf8(JavaVM* jvm, JNIEnv* env, const jstring& value)
		{ return _strParams.MakeSingle(env, value).GetData(); }

		Joint_Value* AllocateMembers(size_t count)
		{ return _members.Make(count); }
	};


	class RetValueAllocator
	{
	public:
		const char* AllocateUtf8(JavaVM* jvm, JNIEnv* env, const jstring& value)
		{
			StringDataHolder str_data(env, value);
			std::unique_ptr<char[]> result_str(new char[strlen(str_data.GetData()) + 1]);
			strcpy(result_str.get(), str_data.GetData());
			return result_str.release();
		}

		Joint_Value* AllocateMembers(size_t count)
		{ return new Joint_Value[count]; }
	};


	class JavaAccessorMarshaller
	{
		JOINT_DEVKIT_LOGGER("Joint.Java.JavaAccessorMarshaller")

	public:
		using JavaVariant = jvalue;

	private:
		JavaVM*    jvm;
		JNIEnv*    env;

	public:
		JavaAccessorMarshaller(JavaVM* jvm, JNIEnv* env)
			: jvm(jvm), env(env)
		{ }

		JavaVariant FromJointBool(Joint_Bool val) const   { jvalue result; result.z = val; return result; }
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

		JavaVariant FromJointEnum(int32_t val, const JavaBindingInfo::TypeUserData& enumType) const
		{
			jvalue result;
			result.l = JAVA_CALL(env->CallStaticObjectMethod(enumType._proxyCls.Get(), enumType._proxyCtorId, (jint)val));
			return result;
		}

		JavaVariant FromJointObj(devkit::ValueDirection dir, Joint_ObjectHandle val, const JavaBindingInfo::TypeUserData& proxyType, Joint_InterfaceChecksum checksum) const
		{
			jvalue result;
			if (val == JOINT_NULL_HANDLE)
			{
				result.l = NULL;
				return result;
			}

			if (dir == devkit::ValueDirection::Parameter)
				Joint_IncRefObject(val);

			auto sg(devkit::ScopeExit([&]{
				if (dir == devkit::ValueDirection::Parameter)
					Joint_DecRefObject(val);
			}));

			JLocalClassPtr cls(env, JAVA_CALL(env->FindClass("org/joint/JointObject")));
			jmethodID ctor_id = JAVA_CALL(env->GetMethodID(cls, "<init>", "(J)V"));
			JLocalObjPtr joint_obj(env, JAVA_CALL(env->NewObject(cls, ctor_id, reinterpret_cast<jlong>(val))));

			result.l = JAVA_CALL(env->NewObject(proxyType._proxyCls.Get(), proxyType._proxyCtorId, joint_obj.Get()));
			sg.Cancel();
			return result;
		}

		JavaVariant FromJointArray(devkit::ValueDirection dir, Joint_ArrayHandle val, const JavaBindingInfo::TypeUserData& elementTypeDesc) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		class ParamsArray
		{
		public:
			void Set(size_t index, JavaVariant param)
			{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		};

		ParamsArray MakeParamsArray(size_t size) const { return ParamsArray(); }

		JavaVariant MakeStruct(const ParamsArray& params, const JavaBindingInfo::TypeUserData& structType) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		Joint_Bool ToJointBool(jvalue val) const  { return val.z; }
		uint8_t    ToJointU8(jvalue val) const    { return val.b; }
		int8_t     ToJointI8(jvalue val) const    { return val.b; }
		uint16_t   ToJointU16(jvalue val) const   { return val.s; }
		int16_t    ToJointI16(jvalue val) const   { return val.s; }
		uint32_t   ToJointU32(jvalue val) const   { return val.i; }
		int32_t    ToJointI32(jvalue val) const   { return val.i; }
		uint64_t   ToJointU64(jvalue val) const   { return val.j; }
		int64_t    ToJointI64(jvalue val) const   { return val.j; }
		float      ToJointF32(jvalue val) const   { return val.f; }
		double     ToJointF64(jvalue val) const   { return val.d; }

		int32_t ToJointEnum(jvalue val, const JavaBindingInfo::TypeUserData& enumType) const
		{
			jfieldID value_id = JAVA_CALL(env->GetFieldID(enumType._proxyCls, "value", "I"));
			return  JAVA_CALL(env->GetIntField(val.l, value_id));
		}

		Joint_ObjectHandle ToJointObj(devkit::ValueDirection dir, jvalue val, const JavaBindingInfo::TypeUserData& objType) const
		{
			if (!val.l)
				return JOINT_NULL_HANDLE;

			JLocalClassPtr cls(env, JAVA_CALL(env->GetObjectClass(val.l)));
			jfieldID obj_id = JAVA_CALL(env->GetFieldID(cls, "obj", "Lorg/joint/JointObject;"));
			JLocalObjPtr joint_obj(env, JAVA_CALL(env->GetObjectField(val.l, obj_id)));

			JLocalClassPtr JointObject_cls(env, JAVA_CALL(env->FindClass("org/joint/JointObject")));
			jfieldID handle_id = JAVA_CALL(env->GetFieldID(JointObject_cls, "handle", "J"));

			auto handle = reinterpret_cast<Joint_ObjectHandle>(JAVA_CALL(env->GetLongField(joint_obj.Get(), handle_id)));
			if (dir == devkit::ValueDirection::Return)
				Joint_IncRefObject(handle);
			return handle;
		}

		Joint_ArrayHandle ToJointArray(devkit::ValueDirection dir, jvalue val, const JavaBindingInfo::TypeUserData& objType) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		template < typename Allocator_ >
		const char* ToJointUtf8(jvalue val, Allocator_& alloc) const
		{ return alloc.AllocateUtf8(jvm, env, reinterpret_cast<jstring>(val.l)); }

		template < typename MemberInfo_ >
		jvalue GetStructMember(jvalue val, size_t i, const MemberInfo_& memberInfo, const JavaBindingInfo::TypeUserData& structType) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
	};


	class JavaProxyMarshaller
	{
		JOINT_DEVKIT_LOGGER("Joint.Java.JavaProxyMarshaller")

	public:
		using JavaVariant = jobject;

	private:
		JNIEnv*    env;

	public:
		JavaProxyMarshaller(JNIEnv* env)
			: env(env)
		{ }

		JavaVariant FromJointUtf8(const char* val) const
		{ return JAVA_CALL(env->NewStringUTF(val)); }

		JavaVariant FromJointEnum(int32_t val, const JavaBindingInfo::TypeUserData& enumType) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		JavaVariant FromJointObj(devkit::ValueDirection dir, Joint_ObjectHandle val, const JavaBindingInfo::TypeUserData& proxyType, Joint_InterfaceChecksum checksum) const
		{
			if (val == JOINT_NULL_HANDLE)
				return NULL;

			if (dir == devkit::ValueDirection::Parameter)
				Joint_IncRefObject(val);

			auto sg(devkit::ScopeExit([&]{
				if (dir == devkit::ValueDirection::Parameter)
					Joint_DecRefObject(val);
			}));

			JLocalClassPtr cls(env, JAVA_CALL(env->FindClass("org/joint/JointObject")));
			jmethodID ctor_id = JAVA_CALL(env->GetMethodID(cls, "<init>", "(J)V"));
			JLocalObjPtr joint_obj(env, JAVA_CALL(env->NewObject(cls, ctor_id, reinterpret_cast<jlong>(val))));

			jobject result = JAVA_CALL(env->NewObject(proxyType._proxyCls.Get(), proxyType._proxyCtorId, joint_obj.Get()));
			sg.Cancel();
			return result;
		}

		JavaVariant FromJointArray(devkit::ValueDirection dir, Joint_ArrayHandle val, const JavaBindingInfo::TypeUserData& elementTypeDesc) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		class ParamsArray
		{
		public:
			void Set(size_t index, JavaVariant param)
			{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		};

		ParamsArray MakeParamsArray(size_t size) const { return ParamsArray(); }

		JavaVariant MakeStruct(const ParamsArray& params, const JavaBindingInfo::TypeUserData& structType) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

#define DETAIL_JOINT_JAVA_PROXY_SIMPLE_TYPE_MARSHALLING(TypeName_, CppType_, JniType_, CallMethod_, ClassName_, ValueGetterName_, MangledType_) \
			CppType_ ToJoint##TypeName_(const JLocalObjPtr& val) const \
			{ \
				JLocalClassPtr cls(env, JAVA_CALL(env->FindClass(ClassName_))); \
				jmethodID method_id = JAVA_CALL(env->GetMethodID(cls, ValueGetterName_, "()" MangledType_)); \
				return static_cast<CppType_>(JAVA_CALL(env->CallMethod_(val.Get(), method_id))); \
			} \
			JavaVariant FromJoint##TypeName_(CppType_ val) const \
			{ \
				JLocalClassPtr cls(env, JAVA_CALL(env->FindClass(ClassName_))); \
				jmethodID ctor_id = JAVA_CALL(env->GetMethodID(cls, "<init>", "(" MangledType_ ")V")); \
				return JAVA_CALL(env->NewObject(cls, ctor_id, (JniType_)val)); \
			}

		DETAIL_JOINT_JAVA_PROXY_SIMPLE_TYPE_MARSHALLING(Bool, Joint_Bool, jboolean, CallBooleanMethod, "java/lang/Boolean", "booleanValue", "Z")
		DETAIL_JOINT_JAVA_PROXY_SIMPLE_TYPE_MARSHALLING(U8,   uint8_t,    jbyte,    CallByteMethod,    "java/lang/Byte",    "byteValue",    "B")
		DETAIL_JOINT_JAVA_PROXY_SIMPLE_TYPE_MARSHALLING(I8,   int8_t,     jbyte,    CallByteMethod,    "java/lang/Byte",    "byteValue",    "B")
		DETAIL_JOINT_JAVA_PROXY_SIMPLE_TYPE_MARSHALLING(U16,  uint16_t,   jshort,   CallShortMethod,   "java/lang/Short",   "shortValue",   "S")
		DETAIL_JOINT_JAVA_PROXY_SIMPLE_TYPE_MARSHALLING(I16,  int16_t,    jshort,   CallShortMethod,   "java/lang/Short",   "shortValue",   "S")
		DETAIL_JOINT_JAVA_PROXY_SIMPLE_TYPE_MARSHALLING(U32,  uint32_t,   jint,     CallIntMethod,     "java/lang/Integer", "intValue",     "I")
		DETAIL_JOINT_JAVA_PROXY_SIMPLE_TYPE_MARSHALLING(I32,  int32_t,    jint,     CallIntMethod,     "java/lang/Integer", "intValue",     "I")
		DETAIL_JOINT_JAVA_PROXY_SIMPLE_TYPE_MARSHALLING(U64,  uint64_t,   jlong,    CallLongMethod,    "java/lang/Long",    "longValue",    "J")
		DETAIL_JOINT_JAVA_PROXY_SIMPLE_TYPE_MARSHALLING(I64,  int64_t,    jlong,    CallLongMethod,    "java/lang/Long",    "longValue",    "J")
		DETAIL_JOINT_JAVA_PROXY_SIMPLE_TYPE_MARSHALLING(F32,  float,      jfloat,   CallFloatMethod,   "java/lang/Float",   "floatValue",   "F")
		DETAIL_JOINT_JAVA_PROXY_SIMPLE_TYPE_MARSHALLING(F64,  double,     jdouble,  CallDoubleMethod,  "java/lang/Double",  "doubleValue",  "D")

#undef DETAIL_JOINT_JAVA_PROXY_SIMPLE_TYPE_MARSHALLING

		int32_t ToJointEnum(const JLocalObjPtr& val, const JavaBindingInfo::TypeUserData& enumType) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		Joint_ObjectHandle ToJointObj(devkit::ValueDirection dir, const JLocalObjPtr& val, const JavaBindingInfo::TypeUserData& objType) const
		{
			if (!val)
				return JOINT_NULL_HANDLE;

			JLocalClassPtr cls(env, JAVA_CALL(env->GetObjectClass(val.Get())));
			jfieldID obj_id = JAVA_CALL(env->GetFieldID(cls, "obj", "Lorg/joint/JointObject;"));
			JLocalObjPtr joint_obj(env, JAVA_CALL(env->GetObjectField(val.Get(), obj_id)));

			JLocalClassPtr JointObject_cls(env, JAVA_CALL(env->FindClass("org/joint/JointObject")));
			jfieldID handle_id = JAVA_CALL(env->GetFieldID(JointObject_cls, "handle", "J"));

			auto handle = reinterpret_cast<Joint_ObjectHandle>(JAVA_CALL(env->GetLongField(joint_obj.Get(), handle_id)));
			if (dir == devkit::ValueDirection::Return)
				Joint_IncRefObject(handle);
			return handle;
		}

		Joint_ArrayHandle ToJointArray(devkit::ValueDirection dir, const JLocalObjPtr& val, const JavaBindingInfo::TypeUserData& objType) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		template < typename Allocator_ >
		const char* ToJointUtf8(const JLocalObjPtr& val, Allocator_& alloc) const
		{ return alloc.AllocateUtf8(val.GetJvm(), val.GetEnv(), reinterpret_cast<jstring>(val.Get())); }

		template < typename MemberInfo_ >
		const JLocalObjPtr& GetStructMember(const JLocalObjPtr& val, size_t i, const MemberInfo_& memberInfo, const JavaBindingInfo::TypeUserData& structType) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
	};

}}

#endif
