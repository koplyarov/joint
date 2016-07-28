#ifndef JOINT_CPP_COMPONENT_HPP
#define JOINT_CPP_COMPONENT_HPP


#include <atomic>

#include <joint.cpp/IJointObject.hpp>
#include <joint.cpp/MetaProgramming.hpp>
#include <joint.cpp/Ptr.hpp>


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

		template < typename RequestedInterface_, typename Interface_, bool Valid_ = IsBaseOf<RequestedInterface_, Interface_>::Value >
		class InterfaceGetterHelper
		{
		public:
			static RequestedInterface_* Get(Interface_* interface)
			{ return NULL; }
		};

		template < typename RequestedInterface_, typename Interface_ >
		class InterfaceGetterHelper<RequestedInterface_, Interface_, true>
		{
		public:
			static RequestedInterface_* Get(Interface_* interface)
			{ return interface; }
		};


		template < typename RequestedInterface_, typename Component_ >
		class InterfaceGetter
		{
		public:
			static RequestedInterface_* Get(Component_* component)
			{
				StaticAssert<
					//IsBaseOf<RequestedInterface_, Component_>::Value
					IsBaseOf<RequestedInterface_, typename Component_::Interface1>::Value ||
					IsBaseOf<RequestedInterface_, typename Component_::Interface2>::Value ||
					IsBaseOf<RequestedInterface_, typename Component_::Interface3>::Value ||
					IsBaseOf<RequestedInterface_, typename Component_::Interface4>::Value ||
					IsBaseOf<RequestedInterface_, typename Component_::Interface5>::Value
				> ERROR_Invalid_RequestedInterface;
				(void)ERROR_Invalid_RequestedInterface;

				RequestedInterface_* result;
	#define DETAIL_JOINT_TRY_GET_IFC(I_) result = InterfaceGetterHelper<RequestedInterface_, typename Component_::Interface ## I_>::Get(component); if (result) return result
			DETAIL_JOINT_TRY_GET_IFC(1);
			DETAIL_JOINT_TRY_GET_IFC(2);
			DETAIL_JOINT_TRY_GET_IFC(3);
			DETAIL_JOINT_TRY_GET_IFC(4);
			DETAIL_JOINT_TRY_GET_IFC(5);
	#undef DETAIL_JOINT_TRY_CAST
				return result;
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
		typedef Interface2_		Interface2;
		typedef Interface3_		Interface3;
		typedef Interface4_		Interface4;
		typedef Interface5_		Interface5;

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
	ComponentType_* MakeComponentImpl()
	{ return new ComponentType_; }

	template < typename ComponentType_, typename Arg1_ >
	ComponentType_* MakeComponentImpl(const Arg1_& arg1)
	{ return new ComponentType_(arg1); }


	template < typename Interface_, typename ComponentType_ >
	Ptr<Interface_> MakeComponentProxy(Joint_ModuleHandle module, ComponentType_* component)
	{
		IJointObject* internal = detail::InterfaceGetter<typename Interface_::ImplInterface, ComponentType_>::Get(component);
		Joint_ObjectHandle obj = JOINT_NULL_HANDLE;
		JOINT_CALL( Joint_CreateObject(module, internal, &obj) );
		printf("!!! %p\n", obj);
		return Ptr<Interface_>(new Interface_(obj));
	}


	template < typename Interface_, typename ComponentType_ >
	Ptr<Interface_>  MakeComponent(Joint_ModuleHandle module)
	{ return MakeComponentProxy<Interface_>(module, MakeComponentImpl<ComponentType_>()); }

	template < typename Interface_, typename ComponentType_, typename Arg1_ >
	Ptr<Interface_> MakeComponent(Joint_ModuleHandle module, const Arg1_& arg1)
	{ return MakeComponentProxy<Interface_>(module, MakeComponentImpl<ComponentType_>(arg1)); }

}

#endif
