#ifndef JOINT_CPP_MODULE_HPP
#define JOINT_CPP_MODULE_HPP


#include <joint.cpp/Ptr.hpp>
#include <joint.cpp/detail/Dummy.hpp>
#include <joint.cpp/detail/JointCall.hpp>
#include <joint/Joint.h>


namespace joint
{

	class Module
	{
	private:
		Joint_ModuleHandle		_module;
		std::string				_bindingName;
		std::string				_moduleName;

	public:
		Module(std::string bindingName, std::string moduleName)
			: _module(JOINT_NULL_HANDLE), _bindingName(std::move(bindingName)), _moduleName(std::move(moduleName))
		{ JOINT_CALL( Joint_LoadModuleByName(_bindingName.c_str(), _moduleName.c_str(), &_module) ); }

		Module(Joint_ModuleHandle module)
			: _module(module)
		{ }

		Module(Module&& other)
			: _module(other._module)
		{ other._module = JOINT_NULL_HANDLE; }

		~Module()
		{
			if (_module != JOINT_NULL_HANDLE)
			{
				Joint_Error ret = Joint_UnloadModule(_module);
				if (ret != JOINT_ERROR_NONE)
					Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C++", "Could not unload module");
			}
		}

		joint::Ptr<joint::IObject> GetRootObject(const std::string& getterName) const
		{
			Joint_ObjectHandle obj;
			JOINT_CALL( Joint_GetRootObject(_module, getterName.c_str(), &obj) );
			return joint::Ptr<joint::IObject>(new joint::IObject(obj));
		}

		template < typename Interface_ >
		joint::Ptr<Interface_> GetRootObject(const std::string& getterName, detail::Dummy d = detail::Dummy()) const
		{ return joint::Cast<Interface_>(GetRootObject(getterName)); }

		Module(const Module& other) = delete;
		Module& operator = (const Module& other) = delete;
	};


	class BindingHolder
	{
	private:
		Joint_BindingHandle		_binding;

	public:
		BindingHolder(Joint_BindingHandle binding)
			: _binding(binding)
		{ }

		~BindingHolder()
		{
			Joint_Error ret = Joint_ReleaseBinding(_binding);
			if (ret != JOINT_ERROR_NONE)
				Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C++", "Joint_ReleaseBinding failed!");
		}

		Module LoadModule(const std::string& moduleName) const
		{
			Joint_ModuleHandle module;
			JOINT_CALL( Joint_LoadModule(_binding, moduleName.c_str(), &module) );
			return Module(module);
		}
	};

}

#endif
