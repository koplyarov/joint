#ifndef JOINT_CPP_COMPONENT_HPP
#define JOINT_CPP_COMPONENT_HPP


#include <atomic>
#include <typeinfo>

#include <joint.cpp/MetaProgramming.hpp>
#include <joint.cpp/Ptr.hpp>
#include <joint.cpp/TypeList.hpp>
#include <joint.cpp/detail/AccessorHolder.hpp>
#include <joint.cpp/detail/Config.hpp>
#include <joint.cpp/detail/NonCopyable.hpp>


#define DETAIL_JOINT_CPP_ASSERT_IMPLEMENTS_INTERFACE(ComponentType_, Interface_) \
    JOINT_CPP_PORTABLE_STATIC_ASSERT((::joint::detail::InheritsInterface<typename ComponentType_::JointInterfaces, Interface_>::Value), "Interface not implemented!", interface_not_implemented)


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


        template < typename InterfacesList_, typename Interface_, bool IsEmpty_ = TypeList_IsEmpty<InterfacesList_>::Value >
        struct InheritsInterface
        {
            typedef typename InterfacesList_::Type Head;
            static const bool Value = IsSame<Head, Interface_>::Value ||
                InheritsInterface<typename Head::BaseInterfaces, Interface_>::Value ||
                InheritsInterface<typename InterfacesList_::NextNode, Interface_>::Value;
        };

        template < typename InterfacesList_, typename Interface_ >
        struct InheritsInterface<InterfacesList_, Interface_, true>
        { static const bool Value = false; };


        template < typename List_, typename Head_ >
        struct InitLinearizedInterfacesListHead
        { typedef Head_ ValueT; };

        template < typename List_ >
        struct InitLinearizedInterfacesListHead<List_, NoneType>
        { typedef typename List_::Type ValueT; };


        template <
            typename InterfacesList_,
            typename HeadType_ = NoneType,
            typename TailNode_ = TypeListEndNode,
            bool InterfacesListEmpty_ = TypeList_IsEmpty<InterfacesList_>::Value
        >
        struct LinearizedInterfacesList
        {
            typedef typename LinearizedInterfacesList<
                typename InterfacesList_::Type::BaseInterfaces,
                typename InitLinearizedInterfacesListHead<InterfacesList_, HeadType_>::ValueT,
                typename LinearizedInterfacesList<
                    typename InterfacesList_::NextNode,
                    NoneType,
                    TailNode_
                >::ValueT
            >::ValueT ValueT;
        };

        template < typename InterfacesList_, typename HeadType_, typename TailNode_ >
        struct LinearizedInterfacesList<InterfacesList_, HeadType_, TailNode_, true>
        { typedef TypeListNode<HeadType_, TailNode_> ValueT; };

        template < typename InterfacesList_, typename TailNode_ >
        struct LinearizedInterfacesList<InterfacesList_, NoneType, TailNode_, true>
        { typedef TailNode_ ValueT; };

        template < typename Interface_, typename AccessorInterface_, bool Match_ = ContainsInterface<AccessorInterface_, Interface_>::Value >
        struct AccessorsHolder_GetAccessor_Impl
        {
            template < typename Tail_ >
            static JointCore_ObjectAccessor Get(JointCore_ObjectAccessor current, const Tail_& tail)
            { return tail.template GetAccessor<Interface_>(); }
        };

        template < typename Interface_, typename AccessorInterface_ >
        struct AccessorsHolder_GetAccessor_Impl<Interface_, AccessorInterface_, true>
        {
            template < typename Tail_ >
            static JointCore_ObjectAccessor Get(JointCore_ObjectAccessor current, const Tail_& tail)
            { return current; }
        };


        template < typename ComponentImpl_, typename InterfacesList_ >
        class AccessorsHolder
        {
            typedef typename InterfacesList_::Type                                       AccessorInterface;
            typedef typename InterfacesList_::Type::template Accessor<ComponentImpl_>    AccessorType;
            typedef AccessorsHolder<ComponentImpl_, typename InterfacesList_::NextNode>  TailAccessors;

        private:
            static JointCore_ObjectAccessorVTable   s_accessorVTable;
            JointCore_ObjectAccessor                _accessor;
            TailAccessors                           _tail;

        public:
            void Init(void* component)
            {
                _accessor.Instance = component;
                _accessor.VTable = &s_accessorVTable;
                _tail.Init(component);
            }

            template < typename Interface_ >
            JointCore_ObjectAccessor GetAccessor() const
            { return AccessorsHolder_GetAccessor_Impl<Interface_, AccessorInterface>::Get(_accessor, _tail); }

            JointCore_Error GetAccessorById(JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectAccessor* outAccessor)
            {
                JointCore_Error ret = AccessorType::InheritsInterface(interfaceId, checksum);
                if (ret == JOINT_CORE_ERROR_NONE || ret == JOINT_CORE_ERROR_INVALID_INTERFACE_CHECKSUM)
                {
                    *outAccessor = _accessor;
                    return ret;
                }
                else
                    return _tail.GetAccessorById(interfaceId, checksum, outAccessor);
            }
        };


        template < typename ComponentImpl_ >
        class AccessorsHolder<ComponentImpl_, joint::detail::TypeListEndNode>
        {
        public:
            void Init(void* component)
            { }

            template < typename Interface_ >
            JointCore_ObjectAccessor GetAccessor() const
            {
                StaticAssert<(sizeof(Interface_) < 0)> ERROR__interface_not_implemented;
                (void)ERROR__interface_not_implemented;
                return JointCore_ObjectAccessor();
            }

            JointCore_Error GetAccessorById(JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectAccessor* outAccessor)
            { return JOINT_CORE_ERROR_CAST_FAILED; }
        };
    }


    template < typename ComponentImpl_ >
    class ComponentWrapper : private detail::NonCopyable
    {
        typedef typename ComponentImpl_::JointInterfaces Interfaces;
        typedef typename detail::LinearizedInterfacesList<Interfaces>::ValueT LinearizedInterfaces;

        typedef detail::AccessorsHolder<ComponentImpl_, LinearizedInterfaces> Accessors;
        typedef detail::AccessorHolder<JointCore_ModuleAccessor> ModuleAccessorHolder;

    private:
        std::atomic<int>          _refCount;
        Accessors                 _accessors;
        ComponentImpl_            _componentImpl;
        ModuleAccessorHolder      _moduleAccessor;

    public:
        ComponentWrapper(JointCore_ModuleAccessor moduleAccessor)
            : _refCount(1), _componentImpl(), _moduleAccessor(moduleAccessor, ModuleAccessorHolder::NewRefTag())
        { Init(); }

        template < typename Arg1_ >
        ComponentWrapper(JointCore_ModuleAccessor moduleAccessor, const Arg1_& arg1)
            : _refCount(1), _componentImpl(arg1), _moduleAccessor(moduleAccessor, ModuleAccessorHolder::NewRefTag())
        { Init(); }

        template < typename Arg1_, typename Arg2_ >
        ComponentWrapper(JointCore_ModuleAccessor moduleAccessor, const Arg1_& arg1, const Arg2_& arg2)
            : _refCount(1), _componentImpl(arg1, arg2), _moduleAccessor(moduleAccessor, ModuleAccessorHolder::NewRefTag())
        { Init(); }

        ComponentImpl_& GetComponentImpl() { return _componentImpl; }
        const ComponentImpl_& GetComponentImpl() const { return _componentImpl; }

        template < typename Interface_ >
        JointCore_ObjectAccessor GetIntefaceAccessor() const
        { return _accessors.template GetAccessor<Interface_>(); }

        template < typename Interface_ >
        static JointCore_ObjectAccessorVTable GetAccessorVTable()
        {
            JointCore_ObjectAccessorVTable result;
            result.AddRef = &ComponentWrapper::AddRef;
            result.Release = &ComponentWrapper::Release;
            result.CastObject = &ComponentWrapper::CastObject;
            result.InvokeMethod = &ComponentWrapper::InvokeMethod<typename Interface_::template Accessor<ComponentImpl_> >;
            return result;
        }

    //private:
        void Init()
        { _accessors.Init(this); }

        static void AddRef(void* component)
        {
            ComponentWrapper* inst = reinterpret_cast<ComponentWrapper*>(component);
            ++inst->_refCount;
        }

        static void Release(void* component)
        {
            ComponentWrapper* inst = reinterpret_cast<ComponentWrapper*>(component);
            if (--inst->_refCount == 0)
                delete inst;
        }

        static JointCore_Error CastObject(void* component, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectAccessor* outAccessor)
        {
            ComponentWrapper* inst = reinterpret_cast<ComponentWrapper*>(component);
            JointCore_Error ret = inst->_accessors.GetAccessorById(interfaceId, checksum, outAccessor);
            if (ret == JOINT_CORE_ERROR_NONE)
                ++inst->_refCount;
            return ret;
        }

        template < typename Accessor_ >
        static JointCore_Error InvokeMethod(void* component, JointCore_SizeT methodId, const JointCore_Parameter* params, JointCore_SizeT paramsCount, JointCore_RetValue* outRetValue)
        {
            ComponentWrapper* inst = reinterpret_cast<ComponentWrapper*>(component);
            return Accessor_::InvokeMethodImpl(&inst->_componentImpl, methodId, params, paramsCount, outRetValue);
        }
    };


    template < typename ComponentImpl_ >
    class ComponentImplPtr
    {
        typedef ComponentWrapper<ComponentImpl_>    WrapperType;

    private:
        WrapperType*    _wrapper;

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
        JointCore_ObjectAccessorVTable AccessorsHolder<ComponentImpl_, InterfacesList_>::s_accessorVTable(ComponentWrapper<ComponentImpl_>::template GetAccessorVTable<typename InterfacesList_::Type>());
    }


    template < typename ComponentType_ >
    ComponentImplPtr<ComponentType_> MakeComponentWrapper(JointCore_ModuleAccessor moduleAccessor)
    { return ComponentImplPtr<ComponentType_>(new ComponentWrapper<ComponentType_>(moduleAccessor)); }

    template < typename ComponentType_, typename Arg1_ >
    ComponentImplPtr<ComponentType_> MakeComponentWrapper(JointCore_ModuleAccessor moduleAccessor, const Arg1_& arg1)
    { return ComponentImplPtr<ComponentType_>(new ComponentWrapper<ComponentType_>(moduleAccessor, arg1)); }

    template < typename ComponentType_, typename Arg1_, typename Arg2_ >
    ComponentImplPtr<ComponentType_> MakeComponentWrapper(JointCore_ModuleAccessor moduleAccessor, const Arg1_& arg1, const Arg2_& arg2)
    { return ComponentImplPtr<ComponentType_>(new ComponentWrapper<ComponentType_>(moduleAccessor, arg1, arg2)); }


    template < typename Interface_, typename ComponentType_ >
    Ptr<Interface_> MakeComponentProxy(const ComponentImplPtr<ComponentType_>& component)
    {
        ComponentWrapper<ComponentType_>::AddRef(component.GetWrapperPtr());
        const JointCore_ObjectAccessor accessor = component.GetWrapperPtr()->template GetIntefaceAccessor<Interface_>();
        return Ptr<Interface_>(Interface_(accessor));
    }


    template < typename Interface_, typename ComponentType_ >
    Ptr<Interface_>  MakeComponent(JointCore_ModuleAccessor moduleAccessor)
    { return MakeComponentProxy<Interface_>(MakeComponentWrapper<ComponentType_>(moduleAccessor)); }

    template < typename Interface_, typename ComponentType_, typename Arg1_ >
    Ptr<Interface_> MakeComponent(JointCore_ModuleAccessor moduleAccessor, const Arg1_& arg1)
    { return MakeComponentProxy<Interface_>(MakeComponentWrapper<ComponentType_>(moduleAccessor, arg1)); }

    template < typename Interface_, typename ComponentType_, typename Arg1_, typename Arg2_ >
    Ptr<Interface_> MakeComponent(JointCore_ModuleAccessor moduleAccessor, const Arg1_& arg1, const Arg2_& arg2)
    { return MakeComponentProxy<Interface_>(MakeComponentWrapper<ComponentType_>(moduleAccessor, arg1, arg2)); }


    template < typename Interface_ >
    JointCore_ObjectAccessor Export(const Ptr<Interface_>& p)
    {
        JointCore_ObjectAccessor accessor = p.Get()->_GetObjectAccessor();
        accessor.VTable->AddRef(accessor.Instance);
        return accessor;
    }


#ifdef _MSC_VER
#   define JOINT_CPP_DLLEXPORT __declspec(dllexport)
#else
#   define JOINT_CPP_DLLEXPORT
#endif

#define JOINT_CPP_ROOT_OBJECT_GETTER(Name_) \
    static joint::IObject_Ptr Detail_##Name_##_Impl(const joint::ModuleContext& moduleContext); \
    \
    extern "C" JOINT_CPP_DLLEXPORT JointCore_ObjectAccessor Name_(JointCore_ModuleAccessor moduleAccessor) \
    { \
        joint::ModuleContext moduleContext(moduleAccessor, joint::ModuleContext::NewRefTag());\
        return joint::Export(Detail_##Name_##_Impl(moduleContext)); \
    } \
    \
    static joint::IObject_Ptr Detail_##Name_##_Impl(const joint::ModuleContext& moduleContext)

}

#endif
