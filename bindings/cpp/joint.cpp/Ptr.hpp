#ifndef BINDINGS_CPP_JOINT_CPP_PTR_HPP
#define BINDINGS_CPP_JOINT_CPP_PTR_HPP


#include <joint.cpp/detail/JointCall.hpp>
#include <joint.cpp/detail/ProxyBase.hpp>


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

		Ptr(Ptr&& other)
			: _raw(other._raw)
		{ other._raw = T_(); }

		Ptr(const Ptr& other)
			: _raw(other._raw)
		{
			if (*this)
				JOINT_CALL(Joint_IncRefObject(_raw._GetObjectHandle()));
		}

		~Ptr()
		{
			if (*this)
			{
				Joint_Error ret = Joint_DecRefObject(_raw._GetObjectHandle());
				if (ret != JOINT_ERROR_NONE)
					Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.C++", "Joint_DecRefObject failed: %s", Joint_ErrorToString(ret));
			}
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
		{ return _raw._GetObjectHandle() != JOINT_NULL_HANDLE; }

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

		Joint_ObjectHandle result_handle;
		Joint_Error ret = Joint_CastObject(src->_GetObjectHandle(), Dst_::_GetInterfaceId(), &result_handle);
		if (ret == JOINT_ERROR_NONE)
			return Ptr<Dst_>(Dst_(result_handle));
		else if (ret == JOINT_ERROR_CAST_FAILED)
			return Ptr<Dst_>();
		else
			throw std::runtime_error(std::string("Joint_CastObject failed: ") + Joint_ErrorToString(ret));
	}

}

#endif
