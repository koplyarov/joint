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

		//TEST_THROWS_ANYTHING(basic->Throw());

		//TEST_EQUALS((int8_t)14, basic->AddI8(2, 12));
		//TEST_EQUALS((uint8_t)14, basic->AddU8(2, 12));
		//TEST_EQUALS((int16_t)14, basic->AddI16(2, 12));
		//TEST_EQUALS((uint16_t)14, basic->AddU16(2, 12));
		TEST_EQUALS((int32_t)14, basic->AddI32(2, 12));
		TEST_EQUALS((uint32_t)14, basic->AddU32(2, 12));

		auto s = joint::Cast<test::IStringTests>(basic);
		TEST_EQUALS(std::string("abcxyz"), s->Concat("abc", "xyz"));

		//TEST_EQUALS((int64_t)14, basic->AddI64(2, 12));
		//TEST_EQUALS((uint64_t)14, basic->AddU64(2, 12));

		//TEST_EQUALS(14.3f, basic->AddF32(2.1f, 12.2f));
		//TEST_EQUALS(14.3, basic->AddF64(2.1, 12.2));

		//TEST_EQUALS(std::string("abcxyz"), basic->Concat("abc", "xyz"));

		//TEST_EQUALS(true, basic->And(true, true));
		//TEST_EQUALS(false, basic->And(true, false));
	}

	void RunObjectTests()
	{
		ScopedTest t(g_engine, "Object tests");

		auto obj = _module.GetRootObject<IObjectTests>("GetBasicTests");
		auto some_obj = obj->ReturnNewObject();

		TEST_THROWS_NOTHING(some_obj->Method());
		TEST_THROWS_NOTHING(obj->InvokeObjectMethod(some_obj));

		auto some_obj_2 = obj->ReturnSameObject(some_obj);
		TEST_THROWS_NOTHING(some_obj_2->Method());
	}
};


int main()
{
	try
	{
		//Tests t(joint::Module("python", "Tests"));
		Tests t(joint::Module("cpp", "tests"));
		t.RunBasicTests();
		//t.RunObjectTests();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
