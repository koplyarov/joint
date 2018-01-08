#ifndef JOINT_CPP_ARRAY_HPP
#define JOINT_CPP_ARRAY_HPP


#include <joint/Joint.h>

#include <joint.cpp/Ptr.hpp>
#include <joint.cpp/Result.hpp>
#include <joint.cpp/detail/ClassContentChecks.hpp>
#include <joint.cpp/detail/Config.hpp>
#include <joint.cpp/detail/JointCall.hpp>


namespace joint
{

    template < typename T_ >
    class Array;


    namespace detail
    {
        JOINT_CPP_DECLARE_METHOD_CHECK(_GetStructDescriptor);

        template < typename T_, typename Enabler_ = void >
        struct ArrayHelper;

#define DETAIL_JOINT_CPP_BASIC_ARRAY_HELPER(CppType_, VariantMember_, JointType_) \
        template < > \
        struct ArrayHelper<CppType_, void> \
        { \
            static const int ArrayDepth = 1; \
            static void FillType(JointCore_Type* storage) { storage->id = JointType_; } \
            static CppType_ FromJointValue(JointCore_Value value) { return value.VariantMember_; } \
            static JOINT_CPP_RET_TYPE(void) SetArrayElement(JointCore_ArrayHandle handle, JointCore_SizeT index, CppType_ value) \
            { \
                JointCore_Value jv; \
                jv.VariantMember_ = value; \
                JOINT_CALL(Joint_ArraySet(handle, index, jv)); \
                JOINT_CPP_RETURN_VOID(); \
            } \
        }

        DETAIL_JOINT_CPP_BASIC_ARRAY_HELPER(int8_t, i8, JOINT_CORE_TYPE_I8);
        DETAIL_JOINT_CPP_BASIC_ARRAY_HELPER(uint8_t, u8, JOINT_CORE_TYPE_U8);
        DETAIL_JOINT_CPP_BASIC_ARRAY_HELPER(int16_t, i16, JOINT_CORE_TYPE_I16);
        DETAIL_JOINT_CPP_BASIC_ARRAY_HELPER(uint16_t, u16, JOINT_CORE_TYPE_U16);
        DETAIL_JOINT_CPP_BASIC_ARRAY_HELPER(int32_t, i32, JOINT_CORE_TYPE_I32);
        DETAIL_JOINT_CPP_BASIC_ARRAY_HELPER(uint32_t, u32, JOINT_CORE_TYPE_U32);
        DETAIL_JOINT_CPP_BASIC_ARRAY_HELPER(int64_t, i64, JOINT_CORE_TYPE_I64);
        DETAIL_JOINT_CPP_BASIC_ARRAY_HELPER(uint64_t, u64, JOINT_CORE_TYPE_U64);
        DETAIL_JOINT_CPP_BASIC_ARRAY_HELPER(float, f32, JOINT_CORE_TYPE_F32);
        DETAIL_JOINT_CPP_BASIC_ARRAY_HELPER(double, f64, JOINT_CORE_TYPE_F64);

#undef DETAIL_JOINT_CPP_BASIC_ARRAY_HELPER


        template < >
        struct ArrayHelper<std::string, void>
        {
            static const int ArrayDepth = 1;

            static void FillType(JointCore_Type* storage)
            { storage->id = JOINT_CORE_TYPE_UTF8; }

            static std::string FromJointValue(JointCore_Value value)
            { return value.utf8; }

            static JOINT_CPP_RET_TYPE(void) SetArrayElement(JointCore_ArrayHandle handle, JointCore_SizeT index, const std::string& value)
            {
                JointCore_Value jv;
                jv.utf8 = value.c_str();
                JOINT_CALL(Joint_ArraySet(handle, index, jv));
                JOINT_CPP_RETURN_VOID();
            }
        };


        template < typename T_ >
        struct ArrayHelper<T_, typename EnableIf<HasMethod__GetStructDescriptor<T_>::Value>::ValueT>
        {
            static const int ArrayDepth = 1;

            static void FillType(JointCore_Type* storage)
            {
                storage->id = JOINT_CORE_TYPE_STRUCT;
                storage->payload.structDescriptor = T_::_GetStructDescriptor();
            }

            static T_ FromJointValue(JointCore_Value value)
            { return T_::_FromJointMembers(value.members); }

            static JOINT_CPP_RET_TYPE(void) SetArrayElement(JointCore_ArrayHandle handle, JointCore_SizeT index, const T_& value)
            {
                JointCore_Value members[T_::_RecursiveMembersCount];
                value._FillMembers(members);

                JointCore_Value jv;
                jv.members = members;
                JOINT_CALL(Joint_ArraySet(handle, index, jv));
                JOINT_CPP_RETURN_VOID();
            }
        };


        template < typename T_ >
        struct ArrayHelper<Array<T_>, void>
        {
            static const int ArrayDepth = ArrayHelper<T_>::ArrayDepth + 1;

            static void FillType(JointCore_Type* storage)
            {
                storage->id = JOINT_CORE_TYPE_ARRAY;
                storage->payload.arrayElementType = storage + 1;
                ArrayHelper<T_>::FillType(storage + 1);
            }

            static Array<T_> FromJointValue(JointCore_Value value)
            {
                Joint_IncRefArray(value.array);
                return Array<T_>(value.array);
            }

            static JOINT_CPP_RET_TYPE(void) SetArrayElement(JointCore_ArrayHandle handle, JointCore_SizeT index, const Array<T_>& value)
            {
                JointCore_Value jv;
                jv.array = value._GetArrayHandle();
                JOINT_CALL(Joint_ArraySet(handle, index, jv));
                JOINT_CPP_RETURN_VOID();
            }
        };


        template < typename T_ >
        struct ArrayHelper<Ptr<T_>, void>
        {
            static const int ArrayDepth = 1;

            static void FillType(JointCore_Type* storage)
            { storage->id = JOINT_CORE_TYPE_OBJ; }

            static Ptr<T_> FromJointValue(JointCore_Value value)
            {
                JOINT_CORE_INCREF_ACCESSOR(value.obj);
                return Ptr<T_>(value.obj);
            }

            static JOINT_CPP_RET_TYPE(void) SetArrayElement(JointCore_ArrayHandle handle, JointCore_SizeT index, const Ptr<T_>& value)
            {
                JointCore_Value jv;
                jv.obj = value->_GetObjectHandle();
                JOINT_CALL(Joint_ArraySet(handle, index, jv));
                JOINT_CPP_RETURN_VOID();
            }
        };
    }


    template < typename T_ >
    class Array
    {
        using Helper = detail::ArrayHelper<T_>;

    private:
        JointCore_ArrayHandle     _handle;

    public:
        explicit Array(JointCore_ArrayHandle handle)
            : _handle(handle)
        { }

#if !JOINT_CPP_CONFIG_NO_EXCEPTIONS
        explicit Array(size_t size = 0)
        {
            JointCore_Type types_storage[Helper::ArrayDepth];
            Helper::FillType(types_storage);
            JOINT_CALL(Joint_MakeArray(types_storage[0], size, &_handle));
        }
#endif

        explicit operator bool() const
        { return _handle; }

        Array(const Array& other)
            : _handle(other._handle)
        { Joint_IncRefArray(_handle); }

        Array& operator = (const Array& other)
        {
            Array tmp(other);
            Swap(tmp);
            return *this;
        }

#if DETAIL_JOINT_CPP_MOVE_SUPPORTED
        Array(Array&& other)
            : _handle(other._handle)
        { Joint_IncRefArray(_handle); }

        Array& operator = (Array&& other)
        {
            Array tmp(other);
            Swap(tmp);
            return *this;
        }
#endif

        ~Array()
        { Joint_DecRefArray(_handle); }


        static JOINT_CPP_RET_TYPE(Array) Create(size_t size = 0)
        {
            JointCore_Type types_storage[Helper::ArrayDepth];
            Helper::FillType(types_storage);
            JointCore_ArrayHandle handle;
            JOINT_CALL(Joint_MakeArray(types_storage[0], size, &handle));
            return Array(handle);
        }


        JOINT_CPP_RET_TYPE(T_) operator[] (size_t index) const
        { return Get(index); }

        JointCore_ArrayHandle _GetArrayHandle() const
        { return _handle; }

        JOINT_CPP_RET_TYPE(size_t) GetSize() const
        {
            JointCore_SizeT result;
            JOINT_CALL( Joint_ArrayGetSize(_handle, &result) );
            return result;
        }

        void Swap(Array& other)
        { std::swap(_handle, other._handle); }


        JOINT_CPP_RET_TYPE(T_) Get(size_t index) const
        {
            JointCore_Value v;
            JOINT_CALL(Joint_ArrayGet(_handle, index, &v));
            return Helper::FromJointValue(v);
        }

        void Set(size_t index, const T_& value)
        { Helper::SetArrayElement(_handle, index, value); }
    };


    template < typename T_ >
    std::ostream& operator << (std::ostream& s, const Array<T_>& a)
    {
        if (a)
            return s << "Array { size: " << a.GetSize() << " }";
        else
            return s << "null";
    }

}

#endif
