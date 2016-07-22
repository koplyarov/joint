#include <iostream>
#include <memory>

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

		auto basic = _module.GetRootObject<IBasicTests>("GetBasicTests");

		TEST_THROWS_ANYTHING(basic->Throw());
		TEST_THROWS_NOTHING(basic->ReturnI32());
		TEST_EQUALS(14, basic->AddI32(2, 12));
	}

	void RunObjectTests()
	{
		ScopedTest t(g_engine, "Object tests");

		auto obj = _module.GetRootObject<IObjectTests>("GetBasicTests");
		auto some_obj = obj->ReturnObject();

		TEST_THROWS_NOTHING(some_obj->Method());
		TEST_THROWS_NOTHING(obj->InvokeObjectMethod(some_obj));
	}
};


int main()
{
	try
	{
		Tests t(joint::Module("python", "Tests"));
		t.RunBasicTests();
		t.RunObjectTests();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
