#ifndef TEST_TESTS_STRUCTTESTS_HPP
#define TEST_TESTS_STRUCTTESTS_HPP


class StructTestsCallback
{
public:
	typedef joint::TypeList<test::IStructTestsCallback>	JointInterfaces;

	test::S1 MakeS1(int32_t i, const std::string& s) { return test::S1(i, s); }
	std::string GetS(test::S1 s) { return s.s; }
	int32_t GetI(test::S1 s) { return s.i; }
};

TEST_DEFINE_TEST(TestCtx, StructTests)
{
	auto t = Module.GetRootObject<test::IStructTests>("GetTests");
	if (!t)
	{
		TEST_REPORT_ERROR("StructTests not implemented!");
		return;
	}

	TEST_EQUALS(t->MakeS1(42, "abc").i, 42);
	TEST_EQUALS(t->MakeS1(42, "abc").s, std::string("abc"));
	TEST_EQUALS(t->GetI(test::S1(42, "abc")), 42);
	TEST_EQUALS(t->GetS(test::S1(42, "abc")), std::string("abc"));

	auto cb = Ctx.MakeComponent<test::IStructTestsCallback, StructTestsCallback>();
	TEST_EQUALS(t->CallbackMakeS1(cb, 42, "abc").i, 42);
	TEST_EQUALS(t->CallbackMakeS1(cb, 42, "abc").s, std::string("abc"));
	TEST_EQUALS(t->CallbackGetI(cb, test::S1(42, "abc")), 42);
	TEST_EQUALS(t->CallbackGetS(cb, test::S1(42, "abc")), std::string("abc"));
}

#endif
