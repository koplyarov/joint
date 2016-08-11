#ifndef JOINT_CPP_MODULE_HPP
#define JOINT_CPP_MODULE_HPP


#include <joint/JointLoader.h>

#include <bindings/cpp/JointCppCore.h>
#include <joint.cpp/Component.hpp>
#include <joint.cpp/Ptr.hpp>
#include <joint.cpp/detail/Dummy.hpp>
#include <joint.cpp/detail/JointCall.hpp>


namespace joint
{

	class ModuleContext
	{
	private:
		Joint_ModuleHandle		_module;

	public:
		ModuleContext(Joint_ModuleHandle module = JOINT_NULL_HANDLE) : _module(module) { }
		ModuleContext(const ModuleContext& other) : _module(other._module) { JOINT_CALL( Joint_IncRefModule(_module) ); }
		ModuleContext& operator= (const ModuleContext& other) { ModuleContext tmp(other); Swap(tmp); return *this; }

#if __cplusplus >= 201100L
		ModuleContext(ModuleContext&& other) : _module(other._module) { other._module = JOINT_NULL_HANDLE; }
		ModuleContext& operator= (ModuleContext&& other) { ModuleContext tmp(other); Swap(tmp); return *this; }
#endif

		~ModuleContext()
		{
			if (_module != JOINT_NULL_HANDLE)
			{
				Joint_Error ret = Joint_DecRefModule(_module);
				if (ret != JOINT_ERROR_NONE)
					Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C++", "Could not unload module");
			}
		}

		Joint_ModuleHandle GetHandle() const
		{ return _module; }

		void Swap(ModuleContext& other)
		{ std::swap(_module, other._module); }

		template < typename Interface_, typename ComponentType_ >
		Ptr<Interface_> MakeComponentProxy(const ComponentImplPtr<ComponentType_>& component)
		{ return joint::MakeComponentProxy<Interface_, ComponentType_>(_module, component); }

		template < typename Interface_, typename ComponentType_ >
		Ptr<Interface_> MakeComponent()
		{ return joint::MakeComponent<Interface_, ComponentType_>(_module); }

		template < typename Interface_, typename ComponentType_, typename Arg1_ >
		Ptr<Interface_> MakeComponent(const Arg1_& arg1)
		{ return joint::MakeComponent<Interface_, ComponentType_, Arg1_>(_module, arg1); }
	};


	class Module
	{
	private:
		Joint_ModuleHandle		_module;

	public:
		Module(std::string bindingName, std::string moduleName)
			: _module(JOINT_NULL_HANDLE)
		{ JOINT_CALL( Joint_LoadModuleByName(bindingName.c_str(), moduleName.c_str(), &_module) ); }

		Module(Joint_ModuleHandle module = JOINT_NULL_HANDLE) : _module(module) { }
		Module(const Module& other) : _module(other._module) { JOINT_CALL( Joint_IncRefModule(_module) ); }
		Module& operator= (const Module& other) { Module tmp(other); Swap(tmp); return *this; }

#if __cplusplus >= 201100L
		Module(Module&& other) : _module(other._module) { other._module = JOINT_NULL_HANDLE; }
		Module& operator= (Module&& other) { Module tmp(other); Swap(tmp); return *this; }
#endif

		~Module()
		{
			if (_module != JOINT_NULL_HANDLE)
			{
				Joint_Error ret = Joint_DecRefModule(_module);
				if (ret != JOINT_ERROR_NONE)
					Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C++", "Could not unload module");
			}
		}

		Joint_ModuleHandle GetHandle() const
		{ return _module; }

		void Swap(Module& other)
		{ std::swap(_module, other._module); }

		joint::Ptr<joint::IObject> GetRootObject(const std::string& getterName) const
		{
			Joint_ObjectHandle obj;
			JOINT_CALL( Joint_GetRootObject(_module, getterName.c_str(), &obj) );
			return joint::Ptr<joint::IObject>(joint::IObject(obj));
		}

		template < typename Interface_ >
		joint::Ptr<Interface_> GetRootObject(const std::string& getterName, detail::Dummy d = detail::Dummy()) const
		{ return joint::Cast<Interface_>(GetRootObject(getterName)); }
	};


	class Binding
	{
	private:
		Joint_BindingHandle		_binding;

	public:
		Binding(Joint_BindingHandle binding = JOINT_NULL_HANDLE) : _binding(binding) { }
		Binding(const Binding& other) : _binding(other._binding) { JOINT_CALL( Joint_IncRefBinding(_binding) ); }
		Binding& operator= (const Binding& other) { Binding tmp(other); Swap(tmp); return *this; }

#if __ReleaseBindingcplusplus >= 201100L
		Binding(Binding&& other) : _binding(other._binding) { other._binding = JOINT_NULL_HANDLE; }
		Binding& operator= (Binding&& other) { Binding tmp(other); Swap(tmp); return *this; }
#endif

		~Binding()
		{
			if (_binding == JOINT_NULL_HANDLE)
				return;

			Joint_Error ret = Joint_DecRefBinding(_binding);
			if (ret != JOINT_ERROR_NONE)
				Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C++", "Joint_ReleaseBinding failed!");
		}

		Joint_BindingHandle GetHandle() const
		{ return _binding; }

		void Swap(Binding& other)
		{ std::swap(_binding, other._binding); }

		Module LoadModule(const std::string& moduleName) const
		{
			Joint_ModuleHandle module;
			JOINT_CALL( Joint_LoadModule(_binding, moduleName.c_str(), &module) );
			return Module(module);
		}
	};


	class Context
	{
	private:
		Binding				_binding;
		ModuleContext		_mainModule;

	public:
		Context()
		{
			Joint_BindingHandle binding_handle = JOINT_NULL_HANDLE;
			JOINT_CALL( JointCppCore_MakeBinding(&binding_handle) );
			_binding = binding_handle;

			Joint_ModuleHandle module_handle = JOINT_NULL_HANDLE;
			JOINT_CALL( Joint_MakeModule(_binding.GetHandle(), NULL, &module_handle) );
			_mainModule = module_handle;
		}

		~Context()
		{ }

		ModuleContext GetMainModule() const { return _mainModule; }

		Module LoadModule(std::string bindingName, std::string moduleName)
		{ return Module(bindingName, moduleName); }

		template < typename Interface_, typename ComponentType_ >
		Ptr<Interface_> MakeComponentProxy(const ComponentImplPtr<ComponentType_>& component)
		{ return _mainModule.MakeComponentProxy<Interface_, ComponentType_>(component); }

		template < typename Interface_, typename ComponentType_ >
		Ptr<Interface_> MakeComponent()
		{ return _mainModule.MakeComponent<Interface_, ComponentType_>(); }

		template < typename Interface_, typename ComponentType_, typename Arg1_ >
		Ptr<Interface_> MakeComponent(const Arg1_& arg1)
		{ return _mainModule.MakeComponent<Interface_, ComponentType_, Arg1_>(arg1); }
	};

}

#endif
