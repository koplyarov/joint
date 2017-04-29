#ifndef TEST_TESTS_STARTERTESTS_HPP
#define TEST_TESTS_STARTERTESTS_HPP


TEST_DEFINE_TEST(TestCtx, StarterTests)
{
	auto t = Module.GetRootObject<test::IStarterTests>("GetTests");
	if (t)
		TEST_EQUALS(t->Increment(12), (int32_t)13);
}


#endif
