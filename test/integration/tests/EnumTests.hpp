#ifndef TEST_TESTS_ENUMTESTS_HPP
#define TEST_TESTS_ENUMTESTS_HPP


TEST_DEFINE_TEST(TestCtx, EnumTests)
{
	auto t = Module.GetRootObject<test::IEnumTests>("GetTests");
	if (!t)
	{
		TEST_REPORT_ERROR("EnumTests not implemented!");
		return;
	}

	test::Enum e;
	TEST_EQUALS(e._RawValue(), test::Enum::Value1);
	TEST_EQUALS(e.ToString(), joint::String("Value1"));

	e = test::Enum::Value3;
	TEST_EQUALS(e._RawValue(), test::Enum::Value3);
	TEST_EQUALS(e.ToString(), joint::String("Value3"));

	TEST_IS_TRUE(t->StringRepresentation(test::Enum::Value1).Storage().find("Value1") != std::string::npos);
	TEST_IS_TRUE(t->StringRepresentation(test::Enum::Value3).Storage().find("Value3") != std::string::npos);
	TEST_EQUALS(t->GetNextValueInRing(test::Enum::Value1)._RawValue(), test::Enum::Value2);
	TEST_EQUALS(t->GetNextValueInRing(test::Enum::Value2)._RawValue(), test::Enum::Value3);
}

#endif
