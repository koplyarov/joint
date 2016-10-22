#ifndef BINDING_MARSHALLER_HPP
#define BINDING_MARSHALLER_HPP


#include <joint/devkit/Logger.hpp>
#include <joint/devkit/StackStorage.hpp>
#include <joint/devkit/ValueMarshaller.hpp>

#include <memory>
#include <string.h>

#include <utils/Utils.hpp>


namespace joint_java
{

	class ParamsAllocator
	{
	private:
		joint::devkit::StackStorage<Joint_Value, 64>        _members;
		joint::devkit::StackStorage<StringDataHolder, 64>   _strParams;

	public:
		const char* AllocateUtf8(const JLocalStringPtr& value)
		{ return _strParams.MakeSingle(value).GetData(); }

		Joint_Value* AllocateMembers(size_t count)
		{ return _members.Make(count); }
	};


	class RetValueAllocator
	{
	public:
		const char* AllocateUtf8(const JLocalStringPtr& value)
		{
			StringDataHolder str_data(value);
			std::unique_ptr<char[]> result_str(new char[strlen(str_data.GetData()) + 1]);
			strcpy(result_str.get(), str_data.GetData());
			return result_str.release();
		}

		Joint_Value* AllocateMembers(size_t count)
		{ return new Joint_Value[count]; }
	};


	class JavaMarshaller
	{
		JOINT_DEVKIT_LOGGER("Joint.Java.JavaMarshaller")

	public:
		using JavaVariant = jvalue;

	public:
		JavaVariant FromJointBool(Joint_Bool val) const   { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		JavaVariant FromJointU8(uint8_t val) const        { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		JavaVariant FromJointI8(int8_t val) const         { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		JavaVariant FromJointU16(uint16_t val) const      { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		JavaVariant FromJointI16(int16_t val) const       { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		JavaVariant FromJointU32(uint32_t val) const      { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		JavaVariant FromJointI32(int32_t val) const       { jvalue result; result.i = val; return result; }
		JavaVariant FromJointU64(uint64_t val) const      { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		JavaVariant FromJointI64(int64_t val) const       { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		JavaVariant FromJointF32(float val) const         { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		JavaVariant FromJointF64(double val) const        { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		JavaVariant FromJointUtf8(const char* val) const  { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		JavaVariant FromJointEnum(int32_t val, const JavaBindingInfo::TypeUserData& enumType) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		JavaVariant FromJointObj(joint::devkit::ValueDirection dir, Joint_ObjectHandle val, const JavaBindingInfo::TypeUserData& proxyType, Joint_InterfaceChecksum checksum) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		JavaVariant FromJointArray(joint::devkit::ValueDirection dir, Joint_ArrayHandle val, const JavaBindingInfo::TypeUserData& elementTypeDesc) const
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

		Joint_Bool ToJointBool(jvalue val) const  { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		uint8_t    ToJointU8(jvalue val) const    { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		int8_t     ToJointI8(jvalue val) const    { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		uint16_t   ToJointU16(jvalue val) const   { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		int16_t    ToJointI16(jvalue val) const   { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		uint32_t   ToJointU32(jvalue val) const   { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		int32_t    ToJointI32(jvalue val) const   { return val.i; }
		uint64_t   ToJointU64(jvalue val) const   { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		int64_t    ToJointI64(jvalue val) const   { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		float      ToJointF32(jvalue val) const   { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
		double     ToJointF64(jvalue val) const   { JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		int32_t ToJointEnum(jvalue val, const JavaBindingInfo::TypeUserData& enumType) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		Joint_ObjectHandle ToJointObj(joint::devkit::ValueDirection dir, jvalue val, const JavaBindingInfo::TypeUserData& objType) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		Joint_ArrayHandle ToJointArray(joint::devkit::ValueDirection dir, jvalue val, const JavaBindingInfo::TypeUserData& objType) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		template < typename Allocator_ >
		const char* ToJointUtf8(jvalue val, Allocator_& alloc) const
		//{ return alloc.AllocateUtf8(val); }
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }

		template < typename MemberInfo_ >
		jvalue GetStructMember(jvalue val, size_t i, const MemberInfo_& memberInfo, const JavaBindingInfo::TypeUserData& structType) const
		{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }
	};

}

#endif
