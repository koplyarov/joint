#ifndef JOINT_DEVKIT_VALUEMARSHALLER_HPP
#define JOINT_DEVKIT_VALUEMARSHALLER_HPP


#include <joint/Joint.h>
#include <joint/devkit/InterfaceDescriptor.hpp>
#include <joint/utils/JointException.hpp>

#include <vector>


namespace joint {
namespace devkit
{

	enum class ValueDirection
	{ Parameter, Return };


	class ValueMarshaller
	{
		JOINT_DEVKIT_LOGGER("Joint.Devkit.ValueMarshaller")

	public:
		template < typename WrappedType_, typename ValueWrapper_, typename UserData_ >
		static WrappedType_ FromJoint(ValueDirection dir, const TypeDescriptor<UserData_>& type, const Joint_Value& value, const ValueWrapper_& wrapper)
		{
			auto jtype = type.GetJointType();
			//printf("_FromJoint: %d\n", jtype.id);
			switch (jtype.id)
			{
			case JOINT_TYPE_BOOL: return wrapper.FromJointBool(value.b);
			case JOINT_TYPE_U8: return wrapper.FromJointU8(value.u8);
			case JOINT_TYPE_I8: return wrapper.FromJointI8(value.i8);
			case JOINT_TYPE_U16: return wrapper.FromJointU16(value.u16);
			case JOINT_TYPE_I16: return wrapper.FromJointI16(value.i16);
			case JOINT_TYPE_U32: return wrapper.FromJointU32(value.u32);
			case JOINT_TYPE_I32: return wrapper.FromJointI32(value.i32);
			case JOINT_TYPE_U64: return wrapper.FromJointU64(value.u64);
			case JOINT_TYPE_I64: return wrapper.FromJointI64(value.i64);
			case JOINT_TYPE_F32: return wrapper.FromJointF32(value.f32);
			case JOINT_TYPE_F64: return wrapper.FromJointF64(value.f64);
			case JOINT_TYPE_UTF8: return wrapper.FromJointUtf8(value.utf8);
			case JOINT_TYPE_ENUM: return wrapper.FromJointEnum(value.e, type.GetUserData());
			case JOINT_TYPE_STRUCT:
				{
					const auto& sd = *jtype.payload.structDescriptor;
					auto struct_ctor_params(wrapper.MakeParamsArray(sd.membersCount));
					for (int32_t i = 0; i < sd.membersCount; ++i)
						struct_ctor_params.Set(i, FromJoint<WrappedType_>(dir, type.GetMemberInfo(i).GetType(), value.members[i], wrapper));
					return wrapper.MakeStruct(struct_ctor_params, type.GetUserData());
				}
				break;
			case JOINT_TYPE_OBJ:
				return wrapper.FromJointObj(dir, value.obj, type.GetUserData(), type.GetJointType().payload.interfaceChecksum);
			case JOINT_TYPE_ARRAY:
				return wrapper.FromJointArray(dir, value.array, type.GetUserData());
			default:
				throw std::runtime_error("Unknown parameter type");
			}
		}

		template < typename Unwrapper_, typename Allocator_, typename UserData_, typename WrappedType_ >
		static Joint_Value ToJoint(ValueDirection dir, const TypeDescriptor<UserData_>& type, const WrappedType_& value, const Unwrapper_& unwrapper, Allocator_& alloc)
		{
			auto jtype = type.GetJointType();
			//printf("_ToJoint: %d\n", jtype.id);
			Joint_Value res = { };
			switch (jtype.id)
			{
			case JOINT_TYPE_BOOL: res.b = unwrapper.ToJointBool(value); break;
			case JOINT_TYPE_U8: res.u8 = unwrapper.ToJointU8(value); break;
			case JOINT_TYPE_I8: res.i8 = unwrapper.ToJointI8(value); break;
			case JOINT_TYPE_U16: res.u16 = unwrapper.ToJointU16(value); break;
			case JOINT_TYPE_I16: res.i16 = unwrapper.ToJointI16(value); break;
			case JOINT_TYPE_U32: res.u32 = unwrapper.ToJointU32(value); break;
			case JOINT_TYPE_I32: res.i32 = unwrapper.ToJointI32(value); break;
			case JOINT_TYPE_U64: res.u64 = unwrapper.ToJointU64(value); break;
			case JOINT_TYPE_I64: res.i64 = unwrapper.ToJointI64(value); break;
			case JOINT_TYPE_F32: res.f32 = unwrapper.ToJointF32(value); break;
			case JOINT_TYPE_F64: res.f64 = unwrapper.ToJointF64(value); break;
			case JOINT_TYPE_ENUM: res.e = unwrapper.ToJointEnum(value, type.GetUserData()); break;
			case JOINT_TYPE_UTF8:
				res.utf8 = unwrapper.ToJointUtf8(value, alloc);
				break;
			case JOINT_TYPE_STRUCT:
				{
					const auto& sd = *jtype.payload.structDescriptor;
					res.members = alloc.AllocateMembers(sd.membersCount);
					for (auto i = 0; i < sd.membersCount; ++i)
						res.members[i] = ToJoint(dir, type.GetMemberInfo(i).GetType(), unwrapper.GetStructMember(value, i, type.GetMemberInfo(i), type.GetUserData()), unwrapper, alloc);
				}
				break;
			case JOINT_TYPE_OBJ:
				res.obj = unwrapper.ToJointObj(dir, value, type.GetUserData());
				break;
			case JOINT_TYPE_ARRAY:
				res.array = unwrapper.ToJointArray(dir, value, type.GetUserData());
				break;
			default:
				JOINT_THROW(std::runtime_error("Unknown type"));
			}

			return res;
		}
	};


}}

#endif
