#ifndef TEST_TESTS_INTERFACECHECKSUMTESTS_HPP
#define TEST_TESTS_INTERFACECHECKSUMTESTS_HPP


static int g_errorsReported = 0;
static void ChecksumTestsLogCallback(Joint_LogLevel logLevel, const char* subsystem, const char* message)
{
	if (logLevel >= JOINT_LOGLEVEL_ERROR)
		++g_errorsReported;
}

#define SUPPRESS_LOGS(...) \
	do { \
		JOINT_CALL(Joint_SetLogCallback(&ChecksumTestsLogCallback)); \
		__VA_ARGS__; \
		JOINT_CALL(Joint_SetLogCallback(nullptr)); \
	} while (false)

#define TEST_LOGS_ERROR(...) \
	do { \
		g_errorsReported = 0; \
		SUPPRESS_LOGS(__VA_ARGS__); \
		TEST_GREATER(g_errorsReported, 0); \
	} while (false)

struct InterfaceChecksumComponent1
{ typedef joint::TypeList<test::IInterfaceCS1> JointInterfaces; };

struct InterfaceChecksumComponent2
{
	typedef joint::TypeList<test::IInterfaceCS2> JointInterfaces;
	void f() { }
};

struct InterfaceChecksumComponent12
{
	typedef joint::TypeList<test::IInterfaceCS1, test::IInterfaceCS2> JointInterfaces;
	void f() { }
};


TEST_DEFINE_TEST(TestCtx, InterfaceChecksumTests)
{
	auto t = Module.GetRootObject<test::IInterfaceChecksumTests>("GetTests");
	if (!t)
	{
		TEST_REPORT_ERROR("InterfaceChecksumTests not implemented!");
		return;
	}

	TEST_THROWS_NOTHING(t->Return1());
	TEST_LOGS_ERROR(TEST_THROWS_ANYTHING(t->Return2()));

	test::IInterfaceCS1_Ptr i12;
	TEST_THROWS_NOTHING(i12 = t->Return12());
	TEST_LOGS_ERROR(TEST_THROWS_ANYTHING(joint::Cast<test::IInterfaceCS2>(i12)));

	auto c1 = Ctx.MakeComponent<test::IInterfaceCS1, InterfaceChecksumComponent1>();
	auto c2 = Ctx.MakeComponent<test::IInterfaceCS2, InterfaceChecksumComponent2>();
	auto c12 = Ctx.MakeComponent<test::IInterfaceCS1, InterfaceChecksumComponent12>();
	TEST_THROWS_NOTHING(t->AcceptCS1(c1));
	//TEST_LOGS_ERROR(TEST_THROWS_ANYTHING(t->AcceptCS2(c2)));
	TEST_LOGS_ERROR(TEST_THROWS_ANYTHING(t->CastToCS2(c12)));
}

#endif
