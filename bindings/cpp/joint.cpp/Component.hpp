#ifndef JOINT_CPP_COMPONENT_HPP
#define JOINT_CPP_COMPONENT_HPP


#include <atomic>
#include <typeinfo>

#include <joint.c/Accessor.h>
#include <joint.cpp/MetaProgramming.hpp>
#include <joint.cpp/Ptr.hpp>
#include <joint.cpp/TypeList.hpp>


namespace joint
{

	namespace detail
	{
		template < typename Ifc_, typename BaseIfc_, typename BaseInterfacesList_ = typename Ifc_::BaseInterfaces, bool IsEmpty_ = TypeList_IsEmpty<BaseInterfacesList_>::Value >
		struct ContainsInterface
		{ static const bool Value = IsSame<Ifc_, BaseIfc_>::Value || ContainsInterface<typename BaseInterfacesList_::Type, BaseIfc_>::Value; };

		template < typename Ifc_, typename BaseIfc_, typename BaseInterfacesList_ >
		struct ContainsInterface<Ifc_, BaseIfc_, BaseInterfacesList_, true>
		{ static const bool Value = IsSame<Ifc_, BaseIfc_>::Value; };


		template < typename List_, typename Ifc_, bool IsEmpty_ = TypeList_IsEmpty<List_>::Value >
		struct AppendToInterfacesList
		{
			typedef typename IfElse<
				ContainsInterface<typename List_::Type, Ifc_>::Value,
				List_,
				TypeListNode<
					typename IfElse<
						ContainsInterface<Ifc_, typename List_::Type>::Value,
						Ifc_,
						typename List_::Type
					>::ValueT,
					typename AppendToInterfacesList<typename List_::NextNode, Ifc_>::ValueT
				>
			>::ValueT ValueT;
		};

		template < typename List_, typename Ifc_ >
		struct AppendToInterfacesList<List_, Ifc_, true>
		{ typedef TypeListNode<Ifc_, TypeListEndNode> ValueT; };


		template < typename L_, typename R_, bool IsEmpty_ = TypeList_IsEmpty<R_>::Value >
		struct InterfacesListMerger
		{ typedef typename InterfacesListMerger<typename AppendToInterfacesList<L_, typename R_::Type>::ValueT, typename R_::NextNode>::ValueT ValueT; };

		template < typename L_, typename R_ >
		struct InterfacesListMerger<L_, R_, true>
		{ typedef L_ ValueT; };


		template < typename Root_, typename Bases_ = typename Root_::BaseInterfaces, bool BasesEmpty_ = TypeList_IsEmpty<Bases_>::Value >
		struct InterfacesTreeLinearizer
		{
			typedef typename InterfacesListMerger<
					typename InterfacesListMerger<
						TypeListNode<Root_, TypeListEndNode>,
						typename InterfacesTreeLinearizer<typename Bases_::Type>::ValueT
					>::ValueT,
					typename InterfacesTreeLinearizer<Root_, typename Bases_::NextNode>::ValueT
				>::ValueT ValueT;
		};

		template < typename Root_, typename Bases_ >
		struct InterfacesTreeLinearizer<Root_, Bases_, true>
		{ typedef TypeListNode<Root_, TypeListEndNode> ValueT; };


		template < typename InterfacesList_, bool IsEmpty_ = TypeList_IsEmpty<InterfacesList_>::Value >
		struct LinearizedInterfacesList
		{
			typedef typename InterfacesListMerger<
					typename InterfacesTreeLinearizer<typename InterfacesList_::Type>::ValueT,
					typename LinearizedInterfacesList<typename InterfacesList_::NextNode>::ValueT
				>::ValueT ValueT;
		};

		template < typename InterfacesList_ >
		struct LinearizedInterfacesList<InterfacesList_, true>
		{ typedef TypeListEndNode ValueT; };


		template < typename ComponentImpl_, typename InterfacesList_ >
		class AccessorsHolder
		{
			typedef typename InterfacesList_::Type::template Accessor<ComponentImpl_>    AccessorType;
			typedef AccessorsHolder<ComponentImpl_, typename InterfacesList_::NextNode>  TailAccessors;

		private:
			static JointC_AccessorVTable   s_accessorVTable;
			JointC_Accessor                _accessor;
			TailAccessors                  _tail;

		public:
			void Init(void* component)
			{
				_accessor.Component = component;
				_accessor.VTable = &s_accessorVTable;
				_tail.Init(component);
			}

			template < typename Interface_ >
			const JointC_Accessor* GetAccessor() const
			{ return &_accessor; }

			bool GetAccessorById(Joint_InterfaceId interfaceId, const JointC_Accessor** outAccessor)
			{
				if (AccessorType::InheritsInterface(interfaceId))
				{
					*outAccessor = &_accessor;
					return true;
				}
				else
					return _tail.GetAccessorById(interfaceId, outAccessor);
			}
		};


		template < typename ComponentImpl_ >
		class AccessorsHolder<ComponentImpl_, joint::detail::TypeListEndNode>
		{
		public:
			void Init(void* component)
			{ }

			bool GetAccessorById(Joint_InterfaceId interfaceId, const JointC_Accessor** outAccessor)
			{ return false; }
		};
	}


	template < typename ComponentImpl_ >
	class ComponentWrapper
	{
		typedef typename ComponentImpl_::JointInterfaces Interfaces;
		typedef typename detail::LinearizedInterfacesList<Interfaces>::ValueT LinearizedInterfaces;

		typedef detail::AccessorsHolder<ComponentImpl_, LinearizedInterfaces> Accessors;

	private:
		std::atomic<int>  _refCount;
		Accessors         _accessors;
		ComponentImpl_    _componentImpl;

	public:
		ComponentWrapper()
			: _refCount(1), _componentImpl()
		{ Init(); }

		template < typename Arg1_ >
		ComponentWrapper(const Arg1_& arg1)
			: _refCount(1), _componentImpl(arg1)
		{ Init(); }

		ComponentImpl_& GetComponentImpl() { return _componentImpl; }
		const ComponentImpl_& GetComponentImpl() const { return _componentImpl; }

		template < typename Interface_ >
		const JointC_Accessor* GetIntefaceAccessor() const
		{ return _accessors.template GetAccessor<Interface_>(); }

		template < typename Interface_ >
		static JointC_AccessorVTable GetAccessorVTable()
		{
			JointC_AccessorVTable result;
			result.AddRef = &ComponentWrapper::AddRef;
			result.Release = &ComponentWrapper::Release;
			result.CastObject = &ComponentWrapper::CastObject;
			result.InvokeMethod = &ComponentWrapper::InvokeMethod<typename Interface_::template Accessor<ComponentImpl_> >;
			return result;
		}

	//private:
		void Init()
		{ _accessors.Init(this); }

		static Joint_Error AddRef(void* component)
		{
			ComponentWrapper* inst = reinterpret_cast<ComponentWrapper*>(component);
			++inst->_refCount;
			return JOINT_ERROR_NONE;
		}

		static Joint_Error Release(void* component)
		{
			ComponentWrapper* inst = reinterpret_cast<ComponentWrapper*>(component);
			if (--inst->_refCount == 0)
				delete inst;
			return JOINT_ERROR_NONE;
		}

		static Joint_Error CastObject(void* component, Joint_InterfaceId interfaceId, const JointC_Accessor** outAccessor)
		{
			ComponentWrapper* inst = reinterpret_cast<ComponentWrapper*>(component);
			Joint_Error ret = inst->_accessors.GetAccessorById(interfaceId, outAccessor) ? JOINT_ERROR_NONE : JOINT_ERROR_CAST_FAILED;
			if (ret == JOINT_ERROR_NONE)
				++inst->_refCount;
			return ret;
		}

		template < typename Accessor_ >
		static Joint_Error InvokeMethod(void* component, Joint_SizeT methodId, const Joint_Parameter* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
		{
			ComponentWrapper* inst = reinterpret_cast<ComponentWrapper*>(component);
			return Accessor_::InvokeMethodImpl(&inst->_componentImpl, methodId, params, paramsCount, retType, outRetValue);
		}
	};


	template < typename ComponentImpl_ >
	class ComponentImplPtr
	{
		typedef ComponentWrapper<ComponentImpl_>	WrapperType;

	private:
		WrapperType*	_wrapper;

	public:
		ComponentImplPtr(WrapperType* wrapper)
			: _wrapper(wrapper)
		{ }

		~ComponentImplPtr()
		{
			if (_wrapper)
				WrapperType::Release(_wrapper);
		}

		ComponentImplPtr(const ComponentImplPtr& other)
			: _wrapper(other._wrapper)
		{
			if (_wrapper)
				WrapperType::AddRef(_wrapper);
		}

		ComponentImplPtr operator= (const ComponentImplPtr& other)
		{
			ComponentImplPtr tmp(other);
			Swap(tmp);
			return *this;
		}

		void Swap(const ComponentImplPtr& other)
		{ std::swap(_wrapper, other._wrapper); }

		ComponentImpl_& operator* () const
		{ return _wrapper->GetComponentImpl(); }

		ComponentImpl_* operator-> () const
		{ return &_wrapper->GetComponentImpl(); }

		WrapperType* GetWrapperPtr() const
		{ return _wrapper; }
	};


	namespace detail
	{
		template < typename ComponentImpl_, typename InterfacesList_ >
		JointC_AccessorVTable AccessorsHolder<ComponentImpl_, InterfacesList_>::s_accessorVTable(ComponentWrapper<ComponentImpl_>::template GetAccessorVTable<typename InterfacesList_::Type>());
	}


	template < typename ComponentType_ >
	ComponentImplPtr<ComponentType_> MakeComponentWrapper()
	{ return ComponentImplPtr<ComponentType_>(new ComponentWrapper<ComponentType_>); }

	template < typename ComponentType_, typename Arg1_ >
	ComponentImplPtr<ComponentType_> MakeComponentWrapper(const Arg1_& arg1)
	{ return ComponentImplPtr<ComponentType_>(new ComponentWrapper<ComponentType_>(arg1)); }


	template < typename Interface_, typename ComponentType_ >
	Ptr<Interface_> MakeComponentProxy(Joint_ModuleHandle module, const ComponentImplPtr<ComponentType_>& component)
	{
		ComponentWrapper<ComponentType_>::AddRef(component.GetWrapperPtr());
		JointC_Accessor* accessor = const_cast<JointC_Accessor*>(component.GetWrapperPtr()->template GetIntefaceAccessor<Interface_>());
		Joint_ObjectHandle obj = JOINT_NULL_HANDLE;
		JOINT_CALL( Joint_CreateObject(module, accessor, &obj) );
		return Ptr<Interface_>(Interface_(obj));
	}


	template < typename Interface_, typename ComponentType_ >
	Ptr<Interface_>  MakeComponent(Joint_ModuleHandle module)
	{ return MakeComponentProxy<Interface_>(module, MakeComponentWrapper<ComponentType_>()); }

	template < typename Interface_, typename ComponentType_, typename Arg1_ >
	Ptr<Interface_> MakeComponent(Joint_ModuleHandle module, const Arg1_& arg1)
	{ return MakeComponentProxy<Interface_>(module, MakeComponentWrapper<ComponentType_>(arg1)); }


	template < typename Interface_ >
	Joint_ObjectHandle Export(const Ptr<Interface_>& p)
	{
		Joint_ObjectHandle handle = p.Get()->_GetObjectHandle();
		JOINT_CALL(Joint_IncRefObject(handle));
		return handle;
	}


}

#endif
