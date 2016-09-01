#include <joint/core/Joint_Array.hpp>

#include <joint/utils/JointException.hpp>

#include <string.h>


extern "C"
{

	Joint_Array::Joint_Array(Joint_Type elementType, std::size_t size)
		: elementType(DeepCopyType(elementType)), elements(size), refCount(1)
	{
		for (auto& e : elements)
			e = MakeDefaultValue(elementType);
	}


	Joint_Array::~Joint_Array()
	{
		for (auto& e : elements)
			ReleaseValue(elementType, e);
		DeepDeleteType(elementType);
	}


	Joint_SizeT Joint_Array::GetSize() const
	{ return elements.size(); }


	void Joint_Array::Set(Joint_SizeT index, Joint_Value value)
	{
		ReleaseValue(elementType, elements[index]);
		elements[index] = DeepCopyValue(elementType, value);
	}


	Joint_Value Joint_Array::Get(Joint_SizeT index) const
	{
		return elements[index];
	}


	void Joint_Array::ReleaseValue(Joint_Type type, Joint_Value value)
	{
		switch (type.id)
		{
		case JOINT_TYPE_UTF8:
			delete[] value.utf8;
			break;
		case JOINT_TYPE_STRUCT:
			{
				auto& sd = *type.payload.structDescriptor;
				for (int32_t i = 0; i < sd.membersCount; ++i)
					ReleaseValue(sd.memberTypes[i], value.members[i]);
				delete[] value.members;
			}
			break;
		case JOINT_TYPE_ARRAY:
			Joint_DecRefArray(value.array);
			break;
		case JOINT_TYPE_OBJ:
			Joint_DecRefObject(value.obj);
			break;
		default:
			break;
		}
	}


	Joint_Value Joint_Array::DeepCopyValue(Joint_Type type, Joint_Value value)
	{
		Joint_Value result = value;
		switch (type.id)
		{
		case JOINT_TYPE_UTF8:
			{
				char* tmp = new char[strlen(value.utf8) + 1];
				strcpy(tmp, value.utf8);
				result.utf8 = tmp;
			}
			break;
		case JOINT_TYPE_STRUCT:
			{
				auto& sd = *type.payload.structDescriptor;
				result.members = new Joint_Value[sd.membersCount];
				for (int32_t i = 0; i < sd.membersCount; ++i)
					result.members[i] = DeepCopyValue(sd.memberTypes[i], value.members[i]);
			}
			break;
		case JOINT_TYPE_ARRAY:
			Joint_IncRefArray(result.array);
			break;
		case JOINT_TYPE_OBJ:
			Joint_IncRefObject(result.obj);
			break;
		default:
			break;
		}
		return result;
	}


	Joint_Value Joint_Array::MakeDefaultValue(Joint_Type type)
	{
		Joint_Value result = Joint_Value();
		switch (type.id)
		{
		case JOINT_TYPE_I32: result.i32 = 0; break;
		case JOINT_TYPE_F32: result.f32 = 0.f; break;
		case JOINT_TYPE_F64: result.f64 = 0.0; break;
		case JOINT_TYPE_UTF8:
			{
				char* tmp = new char[1];
				*tmp = '\0';
				result.utf8 = tmp;
			}
			break;
		case JOINT_TYPE_STRUCT:
			{
				auto& sd = *type.payload.structDescriptor;
				result.members = new Joint_Value[sd.membersCount];
				for (int32_t i = 0; i < sd.membersCount; ++i)
					result.members[i] = MakeDefaultValue(sd.memberTypes[i]);
			}
			break;
		case JOINT_TYPE_ARRAY:
			break;
		default:
			break;
		}
		return result;
	}


	Joint_Type Joint_Array::DeepCopyType(Joint_Type type)
	{
		Joint_Type result;
		result.id = type.id;
		switch (type.id)
		{
		case JOINT_TYPE_OBJ:
			result.payload.interfaceChecksum = type.payload.interfaceChecksum;
			break;
		case JOINT_TYPE_STRUCT:
			{
				result.payload.structDescriptor = new Joint_StructDescriptor;
				auto& src_sd = *type.payload.structDescriptor;
				auto& dst_sd = *result.payload.structDescriptor;
				dst_sd.membersCount = src_sd.membersCount;
				dst_sd.memberTypes = new Joint_Type[dst_sd.membersCount];
				for (int32_t i = 0; i < dst_sd.membersCount; ++i)
					dst_sd.memberTypes[i] = DeepCopyType(src_sd.memberTypes[i]);
			}
			break;
		default:
			break;
		}
		return result;
	}


	void Joint_Array::DeepDeleteType(Joint_Type type)
	{
		if (type.id != JOINT_TYPE_STRUCT)
			return;

		auto& sd = *type.payload.structDescriptor;
		for (int32_t i = 0; i < sd.membersCount; ++i)
			DeepDeleteType(sd.memberTypes[i]);

		delete[] sd.memberTypes;
		delete type.payload.structDescriptor;
	}

}
