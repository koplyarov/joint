#ifndef JOINT_CPP_COMPONENT_HPP
#define JOINT_CPP_COMPONENT_HPP


#include <atomic>

#include <joint.cpp/IJointObject.hpp>


namespace joint
{

	namespace detail
	{
		template < typename Interface_, int N_ >
		class BaseClass : public Interface_
		{
		public:
			joint::IJointObject* TryCast(const char* interfaceId)
			{
				if (Interface_::_Inherits(interfaceId))
					return static_cast<joint::IJointObject*>(this);
				return NULL;
			}
		};

		template < int N_ >
		class BaseClass<void, N_>
		{
		public:
			joint::IJointObject* TryCast(const char* interfaceId)
			{ return NULL; }
		};


		class RefCounter : public virtual IJointObjectLifetime
		{
		private:
			std::atomic<int>	_refCount;

		public:
			RefCounter()
				: _refCount(1)
			{ }

			virtual void _AddRef()
			{ ++_refCount; }

			virtual void _Release()
			{
				if (--_refCount == 0)
					delete this;
			}
		};
	}

	template <
			typename Interface1_ = void,
			typename Interface2_ = void,
			typename Interface3_ = void,
			typename Interface4_ = void,
			typename Interface5_ = void
		>
	class Component :
		public virtual detail::RefCounter,
		public detail::BaseClass<Interface1_, 1>,
		public detail::BaseClass<Interface2_, 2>,
		public detail::BaseClass<Interface3_, 3>,
		public detail::BaseClass<Interface4_, 4>,
		public detail::BaseClass<Interface5_, 5>
	{
	public:
		typedef Interface1_		Interface1;

		virtual void _AddRef()
		{ detail::RefCounter::_AddRef(); }

		virtual void _Release()
		{ detail::RefCounter::_Release(); }

		virtual joint::IJointObject* _DoCastObject(Joint_InterfaceId interfaceId)
		{
			printf("DoCastObject, interfaceId: %s\n", interfaceId);
			joint::IJointObject* result = NULL;
	#define DETAIL_JOINT_TRY_CAST(I_) result = detail::BaseClass<Interface ## I_ ## _, I_>::TryCast(interfaceId); if (result) return result
			DETAIL_JOINT_TRY_CAST(1);
			DETAIL_JOINT_TRY_CAST(2);
			DETAIL_JOINT_TRY_CAST(3);
			DETAIL_JOINT_TRY_CAST(4);
			DETAIL_JOINT_TRY_CAST(5);
	#undef DETAIL_JOINT_TRY_CAST
			return result;
		}
	};


	template < typename ComponentType_ >
	IJointObject* MakeComponent()
	{
		return static_cast<typename ComponentType_::Interface1*>(new ComponentType_);
	}

}

#endif
