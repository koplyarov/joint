#ifndef JOINT_DEVKIT_UTIL_INTRUSIVEREFCOUNTER_HPP
#define JOINT_DEVKIT_UTIL_INTRUSIVEREFCOUNTER_HPP


#include <joint/devkit/Config.hpp>
#include <joint/util/Assert.h>

#include <atomic>


namespace joint {
namespace devkit
{

	template < typename Derived_ >
	class IntrusiveRefCounter
	{
	private:
		std::atomic<int>   _refCount{1};

	public:
		IntrusiveRefCounter() = default;

		IntrusiveRefCounter(const IntrusiveRefCounter&) = delete;
		IntrusiveRefCounter& operator = (const IntrusiveRefCounter&) = delete;

		void AddRef() JOINT_DEVKIT_NOEXCEPT
		{
			int newVal = ++_refCount;
			JOINT_CORE_ASSERT(newVal > 1);
		}

		void Release() JOINT_DEVKIT_NOEXCEPT
		{
			int newVal = --_refCount;
			JOINT_CORE_ASSERT(newVal >= 0);
			if (newVal == 0)
				delete DerivedPtr();
		}

	protected:
		virtual ~IntrusiveRefCounter() = default;

	private:
		Derived_* DerivedPtr() JOINT_DEVKIT_NOEXCEPT
		{ return static_cast<Derived_*>(this); }
	};

}}

#endif
