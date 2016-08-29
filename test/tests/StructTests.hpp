#ifndef TEST_TESTS_STRUCTTESTS_HPP
#define TEST_TESTS_STRUCTTESTS_HPP


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
}

#endif
