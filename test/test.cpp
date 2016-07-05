#include <iostream>
#include <memory>

#include <../bindings/python/JointPython.h>
#include <Tests_adapters.hpp>
#include <test/core/Tests.hpp>


using namespace test;


test::TestEngine		g_engine;

class Tests
{
private:
	joint::Module			_module;

public:
	Tests(joint::Module module)
		: _module(std::move(module))
	{ }

	void RunBasicTests()
	{
		ScopedTest t(g_engine, "Basic tests");

		joint::Ptr<IBasicTests> basic = _module.GetRootObject<IBasicTests>("GetBasicTests");

		TEST_DOES_NOT_THROW(basic->ReturnI32());
		TEST_EQUALS(14, basic->AddI32(2, 12));
	}
};


int main()
{
	try
	{
		Joint_SetLogLevel(JOINT_LOGLEVEL_WARNING);

		JOINT_CALL( JointPython_Register() );

		Tests t(joint::Module("python", "Tests"));
		t.RunBasicTests();

		JOINT_CALL( JointPython_Unregister() );
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
