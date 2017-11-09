#ifndef JOINT_PRIVATE_JOINT_ARRAY_HPP
#define JOINT_PRIVATE_JOINT_ARRAY_HPP


#include <joint/Joint.h>

#include <atomic>
#include <vector>


extern "C"
{

    struct Joint_Array
    {
        JointCore_Type                  elementType;
        std::vector<JointCore_Value>    elements;
        std::atomic<int>            refCount;

        Joint_Array(JointCore_Type elementType, std::size_t size);
        ~Joint_Array();

        Joint_Array(const Joint_Array&) = delete;
        Joint_Array& operator = (const Joint_Array&) = delete;

        JointCore_SizeT GetSize() const;
        void Set(JointCore_SizeT index, JointCore_Value value);
        JointCore_Value Get(JointCore_SizeT index) const;

    private:
        static JointCore_Value DeepCopyValue(JointCore_Type type, JointCore_Value value);
        static void ReleaseValue(JointCore_Type type, JointCore_Value value);
        static JointCore_Value MakeDefaultValue(JointCore_Type type);
        static JointCore_Type DeepCopyType(JointCore_Type type);
        static void DeepDeleteType(JointCore_Type type);
    };

}

#endif
