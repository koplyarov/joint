#ifndef BINDINGS_CPP_JOINT_CPP_PTR_HPP
#define BINDINGS_CPP_JOINT_CPP_PTR_HPP


#include <joint/Joint.h>

#include <cstddef>
#include <joint.cpp/Result.hpp>
#include <joint.cpp/detail/JointCall.hpp>


namespace joint
{

    template < typename T_ >
    class Ptr;


    template < typename T_ >
    class Ref
    {
        template < typename U_ >
        friend class Ptr;

    public:
        typedef T_ Interface;

    private:
        mutable T_ _raw;

    public:
        explicit Ref(T_ raw = T_())
            : _raw(raw)
        { }

        Ref(std::nullptr_t)
            : _raw()
        { }

        explicit Ref(JointCore_ObjectAccessor obj)
            : _raw(obj)
        { }

        explicit operator bool() const
        { return _raw._GetObjectAccessor().Instance != NULL; }

        T_* Get() const { return &_raw; }

        T_* operator -> () const { return &_raw; }

        T_& operator * () const { return _raw; }

        Ptr<T_> Acquire() const;
    };


    template < typename T_ >
    class Ptr : public Ref<T_>
    {
        typedef Ref<T_> Base;

    public:
        typedef T_ Interface;

    public:
        explicit Ptr(T_ raw = T_())
            : Base(raw)
        { }

        Ptr(std::nullptr_t)
            : Base()
        { }

        explicit Ptr(JointCore_ObjectAccessor obj)
            : Base(obj)
        { }

        explicit Ptr(Ref<T_> ref)
            : Base(ref)
        {
            JointCore_ObjectAccessor a = this->_raw._GetObjectAccessor();
            if (a.Instance)
                a.VTable->AddRef(a.Instance);
        }

        Ptr(Ptr&& other)
            : Base(other._raw)
        { other._raw = T_(); }

        Ptr(const Ptr& other)
            : Base(other._raw)
        {
            JointCore_ObjectAccessor a = this->_raw._GetObjectAccessor();
            if (a.Instance)
                a.VTable->AddRef(a.Instance);
        }

        ~Ptr()
        {
            JointCore_ObjectAccessor a = this->_raw._GetObjectAccessor();
            if (a.Instance)
                a.VTable->Release(a.Instance);
        }

        void Swap(Ptr& other)
        { std::swap(this->_raw, other._raw); }

        void Reset(T_ raw = T_())
        {
            Ptr tmp(raw);
            Swap(tmp);
        }

        Ptr& operator = (const Ptr& other)
        {
            Ptr tmp(other);
            Swap(tmp);
            return *this;
        }
    };

    template < typename T_ >
    Ptr<T_> Ref<T_>::Acquire() const
    { return Ptr<T_>(*this); }


    template < typename Dst_, typename Src_ >
    JOINT_CPP_RET_TYPE(Ptr<Dst_>) Cast(const Ref<Src_>& src)
    {
        if (!src)
            return Ptr<Dst_>();

        JointCore_ObjectAccessor result_accessor;
        JointCore_ObjectAccessor a = src.Get()->_GetObjectAccessor();
        JointCore_Error ret = a.VTable->CastObject(a.Instance, Dst_::_GetInterfaceId(), Dst_::_GetInterfaceChecksum(), &result_accessor);
        if (ret == JOINT_CORE_ERROR_NONE)
            return Ptr<Dst_>(Dst_(result_accessor));
        else if (ret == JOINT_CORE_ERROR_CAST_FAILED)
            return Ptr<Dst_>();
        else
            JOINT_CPP_THROW(Exception(std::string("CastObject failed: ") + JointCore_ErrorToString(ret)));
    }

}

#endif
