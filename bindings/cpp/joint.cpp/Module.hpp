#ifndef JOINT_CPP_MODULE_HPP
#define JOINT_CPP_MODULE_HPP


#include <joint.cpp/Ptr.hpp>
#include <joint.cpp/detail/Dummy.hpp>
#include <joint.cpp/detail/JointCall.hpp>


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
		{ JOINT_CALL( Joint_LoadModule(_bindingName.c_str(), _moduleName.c_str(), &_module) ); }

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

}

#endif
