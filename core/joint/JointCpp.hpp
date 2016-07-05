#ifndef JOINT_JOINTCPP_HPP
#define JOINT_JOINTCPP_HPP


#include <joint/Joint.h>

#include <atomic>
#include <stdexcept>


namespace joint
{

#define JOINT_CALL(...) \
		do { \
			Joint_Error ret = (__VA_ARGS__); \
			if (ret != JOINT_ERROR_NONE) \
				throw std::runtime_error(std::string(#__VA_ARGS__ " failed: ") + Joint_ErrorToString(ret)); \
		} while (false)


	class RetValueGuard
	{
	private:
		Joint_RetValue&    _retValue;

	public:
		RetValueGuard(Joint_RetValue& retValue)
			: _retValue(retValue)
		{ }

		~RetValueGuard()
		{
			Joint_Error ret = Joint_ReleaseRetValue(_retValue);
			if (ret != JOINT_ERROR_NONE)
				Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.C++", "Joint_ReleaseRetValue failed: %s", Joint_ErrorToString(ret));
		}
	};


	class ProxyBase
	{
	protected:
		Joint_ObjectHandle		_obj;
		std::atomic<int>		_refCount;

	public:
		ProxyBase(Joint_ObjectHandle obj)
			: _obj(obj), _refCount(1)
		{ }

		virtual ~ProxyBase()
		{
			Joint_Error ret = Joint_DecRefObject(_obj);
			if (ret != JOINT_ERROR_NONE)
				Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.C++", "Joint_DecRefObject failed: %s", Joint_ErrorToString(ret));
		}

		Joint_ObjectHandle _GetObjectHandle() const
		{ return _obj; }

		void _AddRef()
		{ ++_refCount; }

		void _Release()
		{
			if (--_refCount == 0)
				delete this;
		}
	};


	template < typename T_ >
	class Ptr
	{
	private:
		T_*		_raw;

	public:
		explicit Ptr(T_* raw = nullptr)
			: _raw(raw)
		{ }

		Ptr(Ptr&& other)
			: _raw(other._raw)
		{ other._raw = nullptr; }

		Ptr(const Ptr& other)
			: _raw(other._raw)
		{
			if (_raw)
				_raw->_AddRef();
		}

		~Ptr()
		{
			if (_raw)
				_raw->_Release();
		}


		void swap(Ptr& other)
		{ std::swap(_raw, other._raw); }


		Ptr& operator = (const Ptr& other)
		{
			Ptr tmp(other);
			swap(tmp);
			return *this;
		}

		explicit operator bool() const
		{ return _raw != nullptr; }

		T_* get() const
		{ return _raw; }

		T_* operator -> () const
		{ return _raw; }

		T_& operator * () const
		{ return &_raw; }
	};


	template < typename Dst_, typename Src_ >
	Ptr<Dst_> Cast(Ptr<Src_> src)
	{
		if (!src)
			return Ptr<Dst_>();

		ProxyBase *proxy_base = src.get();
		Joint_ObjectHandle result_handle;
		JOINT_CALL(Joint_CastObject(proxy_base->_GetObjectHandle(), Dst_::_GetInterfaceId(), &result_handle));
		return Ptr<Dst_>(new Dst_(result_handle));
	}

}

#endif
