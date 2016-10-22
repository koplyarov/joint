#ifndef JOINT_PRIVATE_JOINT_ARRAY_HPP
#define JOINT_PRIVATE_JOINT_ARRAY_HPP


#include <joint/Joint.h>

#include <atomic>
#include <vector>


extern "C"
{

	struct Joint_Array
	{
		Joint_Type                  elementType;
		std::vector<Joint_Value>    elements;
		std::atomic<int>            refCount;

		Joint_Array(Joint_Type elementType, std::size_t size);
		~Joint_Array();

		Joint_Array(const Joint_Array&) = delete;
		Joint_Array& operator = (const Joint_Array&) = delete;

		Joint_SizeT GetSize() const;
		void Set(Joint_SizeT index, Joint_Value value);
		Joint_Value Get(Joint_SizeT index) const;

	private:
		static Joint_Value DeepCopyValue(Joint_Type type, Joint_Value value);
		static void ReleaseValue(Joint_Type type, Joint_Value value);
		static Joint_Value MakeDefaultValue(Joint_Type type);
		static Joint_Type DeepCopyType(Joint_Type type);
		static void DeepDeleteType(Joint_Type type);
	};

}

#endif
