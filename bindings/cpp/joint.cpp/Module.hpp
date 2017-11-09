#ifndef JOINT_CPP_MODULE_HPP
#define JOINT_CPP_MODULE_HPP


#include <joint/JointLoader.h>

#include <JointNative.h>
#include <joint.cpp/Component.hpp>
#include <joint.cpp/Ptr.hpp>
#include <joint.cpp/Result.hpp>
#include <joint.cpp/detail/Config.hpp>
#include <joint.cpp/detail/Dummy.hpp>
#include <joint.cpp/detail/JointCall.hpp>


namespace joint
{

    class Manifest
    {
    private:
        JointCore_ManifestHandle    _manifest;

    public:
#if !JOINT_CPP_CONFIG_NO_EXCEPTIONS
        Manifest(const std::string& location)
            : _manifest(JOINT_CORE_NULL_HANDLE)
        { JOINT_CALL( Joint_ReadManifestFromFile(location.c_str(), &_manifest) ); }
#endif

        Manifest(JointCore_ManifestHandle handle)
            : _manifest(handle)
        { }

        Manifest(const Manifest& other)
            : _manifest(other._manifest)
        { Joint_IncRefManifest(_manifest); }

        Manifest& operator = (const Manifest& other)
        {
            Manifest tmp(other);
            Swap(tmp);
            return *this;
        }

        ~Manifest()
        { Joint_DecRefManifest(_manifest); }

        static JOINT_CPP_RET_TYPE(Manifest) Create(const std::string& location)
        {
            JointCore_ManifestHandle handle(JOINT_CORE_NULL_HANDLE);
            JOINT_CALL( Joint_ReadManifestFromFile(location.c_str(), &handle) );
            return Manifest(handle);
        }

        JointCore_ManifestHandle GetHandle() const
        { return _manifest; }

    private:
        void Swap(Manifest& other)
        { std::swap(_manifest, other._manifest); }
    };


    class ModuleContext
    {
    private:
        JointCore_ModuleAccessor    _accessor;

    public:
        class NewRefTag { };
        class StealRefTag { };

    public:
        ModuleContext()
        {
            _accessor.VTable = NULL;
            _accessor.Instance = NULL;
        }

        ModuleContext(JointCore_ModuleAccessor accessor, NewRefTag)
            : _accessor(accessor)
        {
            if (_accessor.Instance)
                _accessor.VTable->AddRef(_accessor.Instance);
        }

        ModuleContext(JointCore_ModuleAccessor accessor, StealRefTag)
            : _accessor(accessor)
        { }

        ModuleContext(const ModuleContext& other)
            : _accessor(other._accessor)
        {
            if (_accessor.Instance)
                _accessor.VTable->AddRef(_accessor.Instance);
        }

        ModuleContext& operator= (const ModuleContext& other)
        {
            ModuleContext tmp(other);
            Swap(tmp);
            return *this;
        }

#if DETAIL_JOINT_CPP_MOVE_SUPPORTED
        ModuleContext(ModuleContext&& other)
            : _accessor(other._accessor)
        {
            other._accessor.VTable = NULL;
            other._accessor.Instance = NULL;
        }

        ModuleContext& operator= (ModuleContext&& other)
        {
            ModuleContext tmp(other);
            Swap(tmp);
            return *this;
        }
#endif

        ~ModuleContext()
        {
            if (_accessor.Instance != NULL)
                _accessor.VTable->Release(_accessor.Instance);
        }

        JointCore_ModuleAccessor GetAccessor() const
        { return _accessor; }

        void Swap(ModuleContext& other)
        { std::swap(_accessor, other._accessor); }

        template < typename Interface_, typename ComponentType_ >
        Ptr<Interface_> MakeComponentProxy(const ComponentImplPtr<ComponentType_>& component) const
        {
            DETAIL_JOINT_CPP_ASSERT_IMPLEMENTS_INTERFACE(ComponentType_, Interface_);
            return joint::MakeComponentProxy<Interface_, ComponentType_>(component);
        }

        template < typename Interface_, typename ComponentType_ >
        Ptr<Interface_> MakeComponent() const
        {
            DETAIL_JOINT_CPP_ASSERT_IMPLEMENTS_INTERFACE(ComponentType_, Interface_);
            return joint::MakeComponent<Interface_, ComponentType_>(_accessor);
        }

        template < typename Interface_, typename ComponentType_, typename Arg1_ >
        Ptr<Interface_> MakeComponent(const Arg1_& arg1) const
        {
            DETAIL_JOINT_CPP_ASSERT_IMPLEMENTS_INTERFACE(ComponentType_, Interface_);
            return joint::MakeComponent<Interface_, ComponentType_, Arg1_>(_accessor, arg1);
        }

        template < typename Interface_, typename ComponentType_, typename Arg1_, typename Arg2_ >
        Ptr<Interface_> MakeComponent(const Arg1_& arg1, const Arg2_& arg2) const
        {
            DETAIL_JOINT_CPP_ASSERT_IMPLEMENTS_INTERFACE(ComponentType_, Interface_);
            return joint::MakeComponent<Interface_, ComponentType_, Arg1_, Arg2_>(_accessor, arg1, arg2);
        }

        template < typename ComponentType_ >
        ComponentImplPtr<ComponentType_> MakeComponentWrapper() const
        { return joint::MakeComponentWrapper<ComponentType_>(_accessor); }

        template < typename ComponentType_, typename Arg1_ >
        ComponentImplPtr<ComponentType_> MakeComponentWrapper(const Arg1_& arg1) const
        { return joint::MakeComponentWrapper<ComponentType_, Arg1_>(_accessor, arg1); }
    };


    class Module
    {
    private:
        JointCore_ModuleAccessor     _module;

    public:
#if !JOINT_CPP_CONFIG_NO_EXCEPTIONS
        explicit Module(const Manifest& manifest)
            : _module(CreateImpl(manifest))
        { }

        explicit Module(const std::string& manifestLocation)
            : _module(CreateImpl(Manifest(manifestLocation)))
        { }
#endif

        explicit Module(JointCore_ModuleAccessor accessor)
            : _module(accessor)
        { }

        Module()
        {
            _module.Instance = NULL;
            _module.VTable = NULL;
        }

        Module(const Module& other)
            : _module(other._module)
        { _module.VTable->AddRef(_module.Instance); }

        Module& operator= (const Module& other)
        {
            Module tmp(other);
            Swap(tmp);
            return *this;
        }

#if DETAIL_JOINT_CPP_MOVE_SUPPORTED
        Module(Module&& other)
            : _module(other._module)
        {
            other._module.Instance = NULL;
        }

        Module& operator= (Module&& other)
        {
            Module tmp(other);
            Swap(tmp);
            return *this;
        }
#endif

        ~Module()
        {
            if (_module.Instance)
                _module.VTable->Release(_module.Instance);
        }

        static JOINT_CPP_RET_TYPE(Module) Create(const Manifest& manifest)
        {
            JOINT_CPP_RET_TYPE(JointCore_ModuleAccessor) accessor(CreateImpl(manifest));
            JOINT_CPP_RETHROW(accessor);
            return Module(JOINT_CPP_RET_VALUE(accessor));
        }

        static JOINT_CPP_RET_TYPE(Module) Create(const std::string& manifestLocation)
        {
            JOINT_CPP_RET_TYPE(Manifest) manifest(Manifest::Create(manifestLocation));
            JOINT_CPP_RETHROW(manifest);
            JOINT_CPP_RET_TYPE(JointCore_ModuleAccessor) accessor(CreateImpl(JOINT_CPP_RET_VALUE(manifest)));
            JOINT_CPP_RETHROW(accessor);
            return Module(JOINT_CPP_RET_VALUE(accessor));
        }

        JointCore_ModuleAccessor GetAccessor() const
        { return _module; }

        void Swap(Module& other)
        { std::swap(_module, other._module); }

        JOINT_CPP_RET_TYPE(Ptr<IObject>) GetRootObject(const std::string& getterName) const
        {
            JointCore_ObjectAccessor obj;
            JOINT_CALL( _module.VTable->GetRootObject(_module.Instance, getterName.c_str(), &obj) );
            return Ptr<IObject>(IObject(obj));
        }

        template < typename Interface_ >
        JOINT_CPP_RET_TYPE(Ptr<Interface_>) GetRootObject(const std::string& getterName, detail::Dummy d = detail::Dummy()) const
        {
            JOINT_CPP_RET_TYPE(Ptr<IObject>) obj(GetRootObject(getterName));
            JOINT_CPP_RETHROW(obj);
            return Cast<Interface_>(JOINT_CPP_RET_VALUE(obj));
        }

    private:
        static JOINT_CPP_RET_TYPE(JointCore_ModuleAccessor) CreateImpl(const Manifest& manifest)
        {
            JointCore_ModuleAccessor module;
            module.Instance = NULL;
            module.VTable = NULL;
            JOINT_CALL( JointCore_LoadModule(manifest.GetHandle(), &module) );
            return module;
        }
    };


    class Binding
    {
    private:
        JointCore_BindingAccessor    _binding;

    public:
        Binding()
        {
            _binding.Instance = NULL;
            _binding.VTable = NULL;
        }

        explicit Binding(JointCore_BindingAccessor binding)
            : _binding(binding)
        { }

        Binding(const Binding& other)
            : _binding(other._binding)
        { JOINT_CORE_INCREF_ACCESSOR(_binding); }

        Binding& operator= (const Binding& other)
        {
            Binding tmp(other);
            Swap(tmp);
            return *this;
        }

#if DETAIL_JOINT_CPP_MOVE_SUPPORTED
        Binding(Binding&& other)
            : _binding(other._binding)
        { other._binding.Instance = NULL; }

        Binding& operator= (Binding&& other)
        {
            Binding tmp(other);
            Swap(tmp);
            return *this;
        }
#endif

        ~Binding()
        { JOINT_CORE_DECREF_ACCESSOR(_binding); }

        JointCore_BindingAccessor GetAccessor() const
        { return _binding; }

        void Swap(Binding& other)
        { std::swap(_binding, other._binding); }
    };


    class Context
    {
    private:
        Binding            _binding;
        ModuleContext      _mainModule;

    public:
#if !JOINT_CPP_CONFIG_NO_EXCEPTIONS
        Context()
        {
            JointCore_BindingAccessor binding_accessor;
            JOINT_CALL( JointNative_MakeBinding(&binding_accessor) );
            _binding = Binding(binding_accessor);
            // TODO: _mainModule?
        }
#endif

        ~Context()
        { }

        static JOINT_CPP_RET_TYPE(Context) Create()
        {
            JointCore_BindingAccessor binding_accessor;
            JOINT_CALL( JointNative_MakeBinding(&binding_accessor) );
            return Context(Binding(binding_accessor)); // TODO: _mainModule?
        }

        ModuleContext GetMainModule() const { return _mainModule; }

        JOINT_CPP_RET_TYPE(Module) LoadModule(const Manifest& manifest)
        { return Module::Create(manifest); }

        template < typename Interface_, typename ComponentType_ >
        Ptr<Interface_> MakeComponentProxy(const ComponentImplPtr<ComponentType_>& component)
        {
            DETAIL_JOINT_CPP_ASSERT_IMPLEMENTS_INTERFACE(ComponentType_, Interface_);
            return _mainModule.MakeComponentProxy<Interface_, ComponentType_>(component);
        }

        template < typename Interface_, typename ComponentType_ >
        Ptr<Interface_> MakeComponent()
        {
            DETAIL_JOINT_CPP_ASSERT_IMPLEMENTS_INTERFACE(ComponentType_, Interface_);
            return _mainModule.MakeComponent<Interface_, ComponentType_>();
        }

        template < typename Interface_, typename ComponentType_, typename Arg1_ >
        Ptr<Interface_> MakeComponent(const Arg1_& arg1)
        {
            DETAIL_JOINT_CPP_ASSERT_IMPLEMENTS_INTERFACE(ComponentType_, Interface_);
            return _mainModule.MakeComponent<Interface_, ComponentType_, Arg1_>(arg1);
        }

        template < typename Interface_, typename ComponentType_, typename Arg1_, typename Arg2_ >
        Ptr<Interface_> MakeComponent(const Arg1_& arg1, const Arg2_& arg2)
        {
            DETAIL_JOINT_CPP_ASSERT_IMPLEMENTS_INTERFACE(ComponentType_, Interface_);
            return _mainModule.MakeComponent<Interface_, ComponentType_, Arg1_, Arg2_>(arg1, arg2);
        }

        template < typename ComponentType_ >
        ComponentImplPtr<ComponentType_> MakeComponentWrapper()
        { return _mainModule.MakeComponentWrapper<ComponentType_>(); }

        template < typename ComponentType_, typename Arg1_ >
        ComponentImplPtr<ComponentType_> MakeComponentWrapper(const Arg1_& arg1)
        { return _mainModule.MakeComponentWrapper<ComponentType_, Arg1_>(arg1); }

    private:
        Context(const Binding& binding)
            : _binding(binding)
        { }
    };

}

#endif
