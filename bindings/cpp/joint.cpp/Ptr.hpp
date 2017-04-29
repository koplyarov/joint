#ifndef BINDINGS_CPP_JOINT_CPP_PTR_HPP
#define BINDINGS_CPP_JOINT_CPP_PTR_HPP


#include <joint.cpp/detail/JointCall.hpp>

#include <cstddef>


namespace joint
{

	template < typename T_ >
	class Ptr
	{
	private:
		mutable T_		_raw;

	public:
		explicit Ptr(T_ raw = T_())
			: _raw(raw)
		{ }

		Ptr(std::nullptr_t)
			: _raw()
		{ }

		explicit Ptr(JointCore_ObjectHandle obj)
			: _raw(obj)
		{ }

		Ptr(Ptr&& other)
			: _raw(other._raw)
		{ other._raw = T_(); }

		Ptr(const Ptr& other)
			: _raw(other._raw)
		{
			if (*this)
				Joint_IncRefObject(_raw._GetObjectHandle());
		}

		~Ptr()
		{
			if (*this)
				Joint_DecRefObject(_raw._GetObjectHandle());
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
		{ return _raw._GetObjectHandle() != JOINT_CORE_NULL_HANDLE; }

		//T_* Get() { return &_raw; }
		T_* Get() const { return &_raw; }

		//T_* operator -> () { return &_raw; }
		T_* operator -> () const { return &_raw; }

		//T_& operator * () { return _raw; }
		T_& operator * () const { return _raw; }
	};


	template < typename Dst_, typename Src_ >
	Ptr<Dst_> Cast(const Ptr<Src_>& src)
	{
		if (!src)
			return Ptr<Dst_>();

		JointCore_ObjectHandle result_handle;
		JointCore_Error ret = Joint_CastObject(src->_GetObjectHandle(), Dst_::_GetInterfaceId(), Dst_::_GetInterfaceChecksum(), &result_handle);
		if (ret == JOINT_CORE_ERROR_NONE)
			return Ptr<Dst_>(Dst_(result_handle));
		else if (ret == JOINT_CORE_ERROR_CAST_FAILED)
			return Ptr<Dst_>();
		else
			throw std::runtime_error(std::string("Joint_CastObject failed: ") + JointCore_ErrorToString(ret));
	}

}

#endif
