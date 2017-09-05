#ifndef JOINT_CPP_MODULE_HPP
#define JOINT_CPP_MODULE_HPP


#include <joint/JointLoader.h>

#include <bindings/native/JointNative.h>
#include <joint.cpp/Component.hpp>
#include <joint.cpp/Ptr.hpp>
#include <joint.cpp/detail/Dummy.hpp>
#include <joint.cpp/detail/JointCall.hpp>


namespace joint
{

	class Manifest
	{
	private:
		JointCore_ManifestHandle    _manifest;

	public:
		Manifest(const std::string& location)
			: _manifest(JOINT_CORE_NULL_HANDLE)
		{ JOINT_CALL( Joint_ReadManifestFromFile(location.c_str(), &_manifest) ); }

		~Manifest()
		{
			if (_manifest != JOINT_CORE_NULL_HANDLE)
				Joint_DeleteManifest(_manifest);
		}

		JointCore_ManifestHandle GetHandle() const
		{ return _manifest; }

	private:
		Manifest(const Manifest&);
		Manifest& operator = (const Manifest&);
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
		{ _accessor.VTable->AddRef(_accessor.Instance); }

		ModuleContext(JointCore_ModuleAccessor accessor, StealRefTag)
			: _accessor(accessor)
		{ }

		ModuleContext(const ModuleContext& other)
			: _accessor(other._accessor)
		{ _accessor.VTable->AddRef(_accessor.Instance); }

		ModuleContext& operator= (const ModuleContext& other)
		{
			ModuleContext tmp(other);
			Swap(tmp);
			return *this;
		}

#if __cplusplus >= 201100L
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
		Module(const Manifest& manifest)
		{ Init(manifest); }

		Module(const std::string& manifestLocation)
		{ Init(Manifest(manifestLocation)); }

		Module()
		{
			_module.Instance = NULL;
			_module.VTable = NULL;
		}

		Module(JointCore_ModuleAccessor module) : _module(module) { }

		Module(const Module& other)
			: _module(other._module)
		{ _module.VTable->AddRef(_module.Instance); }

		Module& operator= (const Module& other)
		{
			Module tmp(other);
			Swap(tmp);
			return *this;
		}

#if __cplusplus >= 201100L
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

		JointCore_ModuleAccessor GetAccessor() const
		{ return _module; }

		void Swap(Module& other)
		{ std::swap(_module, other._module); }

		joint::Ptr<joint::IObject> GetRootObject(const std::string& getterName) const
		{
			JointCore_ObjectAccessor obj;
			JOINT_CALL( _module.VTable->GetRootObject(_module.Instance, getterName.c_str(), &obj) );
			return joint::Ptr<joint::IObject>(joint::IObject(obj));
		}

		template < typename Interface_ >
		joint::Ptr<Interface_> GetRootObject(const std::string& getterName, detail::Dummy d = detail::Dummy()) const
		{ return joint::Cast<Interface_>(GetRootObject(getterName)); }

	private:
		void Init(const Manifest& manifest)
		{
			_module.Instance = NULL;
			_module.VTable = NULL;
			JOINT_CALL( JointCore_LoadModule(manifest.GetHandle(), &_module) );
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

		Binding(JointCore_BindingAccessor binding)
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

#if __ReleaseBindingcplusplus >= 201100L
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
		Context()
		{
			JointCore_BindingAccessor binding_accessor;
			JOINT_CALL( JointNative_MakeBinding(&binding_accessor) );
			_binding = binding_accessor;
			// TODO: _mainModule?
		}

		~Context()
		{ }

		ModuleContext GetMainModule() const { return _mainModule; }

		Module LoadModule(const Manifest& manifest)
		{ return Module(manifest); }

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

		template < typename ComponentType_ >
		ComponentImplPtr<ComponentType_> MakeComponentWrapper()
		{ return _mainModule.MakeComponentWrapper<ComponentType_>(); }

		template < typename ComponentType_, typename Arg1_ >
		ComponentImplPtr<ComponentType_> MakeComponentWrapper(const Arg1_& arg1)
		{ return _mainModule.MakeComponentWrapper<ComponentType_, Arg1_>(arg1); }
	};

}

#endif
