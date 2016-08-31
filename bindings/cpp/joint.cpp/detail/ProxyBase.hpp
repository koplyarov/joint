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

	private:
		ProxyBase(const ProxyBase&);
		ProxyBase& operator = (const ProxyBase&);

	public:
		ProxyBase(Joint_ObjectHandle obj)
			: _obj(obj), _refCount(1)
		{ }

		virtual ~ProxyBase()
		{ Joint_DecRefObject(_obj); }

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
