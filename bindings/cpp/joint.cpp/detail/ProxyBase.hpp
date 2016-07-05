#ifndef BINDINGS_CPP_JOINT_CPP_DETAIL_PROXYBASE_HPP
#define BINDINGS_CPP_JOINT_CPP_DETAIL_PROXYBASE_HPP


#include <joint/Joint.h>

#include <atomic>


namespace joint {
namespace detail
{

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

}}

#endif
