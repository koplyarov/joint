#include <iostream>
#include <map>
#include <memory>

#include <Tests_adapters.hpp>
#include <test/core/Tests.hpp>


std::string g_bindingName = "cpp";
std::string g_moduleName = "tests";

struct TestCtx
{
	joint::Context			Ctx;
	joint::Module			Module;

	TestCtx()
		: Module(Ctx.LoadModule(g_bindingName, g_moduleName))
	{ }
};


class SomeObject
{
public:
	typedef joint::TypeList<test::ISomeObject>	JointInterfaces;

	void Method()
	{ printf("HAHAHAHA\n"); }
};


TEST_DEFINE_TEST(TestCtx, BasicTests)
{
	auto basic = Module.GetRootObject<test::IBasicTests>("GetBasicTests");

	TEST_THROWS_ANYTHING(basic->Throw());

	TEST_EQUALS((int8_t)14, basic->AddI8(2, 12));
	TEST_EQUALS((uint8_t)14, basic->AddU8(2, 12));
	TEST_EQUALS((int16_t)14, basic->AddI16(2, 12));
	TEST_EQUALS((uint16_t)14, basic->AddU16(2, 12));
	TEST_EQUALS((int32_t)14, basic->AddI32(2, 12));
	TEST_EQUALS((uint32_t)14, basic->AddU32(2, 12));

	TEST_EQUALS((int64_t)14, basic->AddI64(2, 12));
	TEST_EQUALS((uint64_t)14, basic->AddU64(2, 12));

	TEST_EQUALS(14.3f, basic->AddF32(2.1f, 12.2f));
	TEST_EQUALS(14.3, basic->AddF64(2.1, 12.2));

	TEST_EQUALS(std::string("abcxyz"), basic->Concat("abc", "xyz"));

	TEST_EQUALS(true, basic->And(true, true));
	TEST_EQUALS(false, basic->And(true, false));
}


TEST_DEFINE_TEST(TestCtx, ObjectTests)
{
	auto obj = Module.GetRootObject<test::IObjectTests>("GetBasicTests");
	auto some_obj = obj->ReturnNewObject();

	TEST_THROWS_NOTHING(some_obj->Method());
	TEST_THROWS_NOTHING(obj->InvokeObjectMethod(some_obj));

	auto some_obj_2 = obj->ReturnSameObject(some_obj);
	TEST_THROWS_NOTHING(some_obj_2->Method());

	auto main_module_some_obj = Ctx.GetMainModule().MakeComponent<test::ISomeObject, SomeObject>();
	TEST_THROWS_NOTHING(obj->InvokeObjectMethod(main_module_some_obj));
}


int main()
{
	try
	{ test::RunAllTests(); }
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
