#ifndef TEST_TESTS_STRUCTTESTS_HPP
#define TEST_TESTS_STRUCTTESTS_HPP


class StructTestsCallback
{
public:
	typedef joint::TypeList<test::IStructTestsCallback> JointInterfaces;

	test::S1 MakeS1(int32_t i, const std::string& s) { return test::S1(i, s); }
	std::string GetSFromS1(test::S1 s) { return s.s; }
	int32_t GetIFromS1(test::S1 s) { return s.i; }

	test::S2 MakeS2(int32_t i, test::S2L s2l, float f, test::S2M s2m, const std::string& s, test::S2R s2r) { return test::S2(i, s2l, f, s2m, s, s2r); }
	int32_t GetIFromS2(test::S2 s) { return s.i; }
	test::S2L GetS2LFromS2(test::S2 s) { return s.s2l; }
	float GetFFromS2(test::S2 s) { return s.f; }
	test::S2M GetS2MFromS2(test::S2 s) { return s.s2m; }
	std::string GetSFromS2(test::S2 s) { return s.s; }
	test::S2R GetS2RFromS2(test::S2 s) { return s.s2r; }
};

TEST_DEFINE_TEST(TestCtx, StructTests)
{
	using namespace test;

	auto t = Module.GetRootObject<IStructTests>("GetTests");
	if (!t)
	{
		TEST_REPORT_ERROR("StructTests not implemented!");
		return;
	}

	TEST_EQUALS(t->MakeS1(42, "abc").i, 42);
	TEST_EQUALS(t->MakeS1(42, "abc").s, std::string("abc"));
	TEST_EQUALS(t->GetIFromS1(S1(42, "abc")), 42);
	TEST_EQUALS(t->GetSFromS1(S1(42, "abc")), std::string("abc"));

	auto cb = Ctx.MakeComponent<IStructTestsCallback, StructTestsCallback>();
	TEST_EQUALS(t->CallbackMakeS1(cb, 42, "abc").i, 42);
	TEST_EQUALS(t->CallbackMakeS1(cb, 42, "abc").s, std::string("abc"));
	TEST_EQUALS(t->CallbackGetIFromS1(cb, S1(42, "abc")), 42);
	TEST_EQUALS(t->CallbackGetSFromS1(cb, S1(42, "abc")), std::string("abc"));

	S2 s2;
	TEST_THROWS_NOTHING(s2 = t->MakeS2(42, S2L{123}, 3.14f, S2M{2.72f}, "abc", S2R{"qwe"}));
	TEST_EQUALS(s2.i, 42);
	TEST_EQUALS(s2.s2l.i, 123);
	TEST_EQUALS(s2.f, 3.14f);
	TEST_EQUALS(s2.s2m.f, 2.72f);
	TEST_EQUALS(s2.s, std::string("abc"));
	TEST_EQUALS(s2.s2r.s, std::string("qwe"));

	TEST_EQUALS(t->GetIFromS2(s2), 42);
	TEST_EQUALS(t->GetS2LFromS2(s2).i, 123);
	TEST_EQUALS(t->GetFFromS2(s2), 3.14f);
	TEST_EQUALS(t->GetS2MFromS2(s2).f, 2.72f);
	TEST_EQUALS(t->GetSFromS2(s2), std::string("abc"));
	TEST_EQUALS(t->GetS2RFromS2(s2).s, std::string("qwe"));

	TEST_THROWS_NOTHING(s2 = t->CallbackMakeS2(cb, 1, S2L{2}, 1.1f, S2M{2.3f}, "AAA", S2R{"ZZZ"}));
	TEST_EQUALS(s2.i, 1);
	TEST_EQUALS(s2.s2l.i, 2);
	TEST_EQUALS(s2.f, 1.1f);
	TEST_EQUALS(s2.s2m.f, 2.3f);
	TEST_EQUALS(s2.s, std::string("AAA"));
	TEST_EQUALS(s2.s2r.s, std::string("ZZZ"));

	TEST_EQUALS(t->CallbackGetIFromS2(cb, s2), 1);
	TEST_EQUALS(t->CallbackGetS2LFromS2(cb, s2).i, 2);
	TEST_EQUALS(t->CallbackGetFFromS2(cb, s2), 1.1f);
	TEST_EQUALS(t->CallbackGetS2MFromS2(cb, s2).f, 2.3f);
	TEST_EQUALS(t->CallbackGetSFromS2(cb, s2), std::string("AAA"));
	TEST_EQUALS(t->CallbackGetS2RFromS2(cb, s2).s, std::string("ZZZ"));
}

#endif
