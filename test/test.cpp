#include <iostream>
#include <memory>

#include <../bindings/python/JointPython.h>
#include <Tests_adapters.hpp>


int main()
{
	try
	{
		using namespace test;

		JOINT_CALL( JointPython_Register() );

		{
			joint::Module m("python", "Tests");

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
