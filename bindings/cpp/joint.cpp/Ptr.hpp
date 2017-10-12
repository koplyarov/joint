#ifndef BINDINGS_CPP_JOINT_CPP_PTR_HPP
#define BINDINGS_CPP_JOINT_CPP_PTR_HPP


#include <joint/Joint.h>

#include <cstddef>
#include <joint.cpp/Result.hpp>
#include <joint.cpp/detail/JointCall.hpp>


namespace joint
{

	template < typename T_ >
	class Ptr
	{
	public:
		typedef T_ Interface;

	private:
		mutable T_ _raw;

	public:
		explicit Ptr(T_ raw = T_())
			: _raw(raw)
		{ }

		Ptr(std::nullptr_t)
			: _raw()
		{ }

		explicit Ptr(JointCore_ObjectAccessor obj)
			: _raw(obj)
		{ }

		Ptr(Ptr&& other)
			: _raw(other._raw)
		{ other._raw = T_(); }

		Ptr(const Ptr& other)
			: _raw(other._raw)
		{
			JointCore_ObjectAccessor a = _raw._GetObjectAccessor();
			if (a.Instance)
				a.VTable->AddRef(a.Instance);
		}

		~Ptr()
		{
			JointCore_ObjectAccessor a = _raw._GetObjectAccessor();
			if (a.Instance)
				a.VTable->Release(a.Instance);
		}


		void Swap(Ptr& other)
		{ std::swap(_raw, other._raw); }

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

		explicit operator bool() const
		{ return _raw._GetObjectAccessor().Instance != NULL; }

		T_* Get() const { return &_raw; }

		T_* operator -> () const { return &_raw; }

		T_& operator * () const { return _raw; }
	};


	template < typename Dst_, typename Src_ >
	JOINT_CPP_RET_TYPE(Ptr<Dst_>) Cast(const Ptr<Src_>& src)
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
