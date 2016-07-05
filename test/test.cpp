#include <joint/JointCpp.hpp>

#include <iostream>
#include <memory>

#include <../bindings/python/JointPython.h>
#include <Tests_adapters.hpp>


class Dummy { };


class JointModule
{
private:
	Joint_ModuleHandle		_module;
	std::string				_bindingName;
	std::string				_moduleName;

public:
	JointModule(std::string bindingName, std::string moduleName)
		: _module(JOINT_NULL_HANDLE), _bindingName(std::move(bindingName)), _moduleName(std::move(moduleName))
	{ JOINT_CALL( Joint_LoadModule(_bindingName.c_str(), _moduleName.c_str(), &_module) ); }

	JointModule(JointModule&& other)
		: _module(other._module)
	{ other._module = JOINT_NULL_HANDLE; }

	~JointModule()
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
	joint::Ptr<Interface_> GetRootObject(const std::string& getterName, Dummy d = Dummy()) const
	{ return joint::Cast<Interface_>(GetRootObject(getterName)); }

	JointModule(const JointModule& other) = delete;
	JointModule& operator = (const JointModule& other) = delete;
};


int main()
{
	try
	{
		using namespace test;

		JOINT_CALL( JointPython_Register() );

		{
			JointModule m("python", "Tests");

			joint::Ptr<IBasicTests> basic = m.GetRootObject<IBasicTests>("GetBasicTests");
			std::cout << "i32: " << basic->ReturnI32() << std::endl;
			std::cout << "sum: " << basic->AddI32(2, 12) << std::endl;
		}

		JOINT_CALL( JointPython_Unregister() );
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
