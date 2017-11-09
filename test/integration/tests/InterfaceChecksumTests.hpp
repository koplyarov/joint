#ifndef TEST_TESTS_INTERFACECHECKSUMTESTS_HPP
#define TEST_TESTS_INTERFACECHECKSUMTESTS_HPP


#include <algorithm>


static bool g_errorsReported = false, g_gotInterfaceWord = false, g_gotChecksumWord = false;
static void ChecksumTestsLogCallback(JointCore_LogLevel logLevel, const char* subsystem, const char* message)
{
    if (logLevel >= JOINT_CORE_LOGLEVEL_ERROR)
        g_errorsReported = true;

    std::string msg(message);
    std::transform(msg.begin(), msg.end(), msg.begin(), ::tolower);

    g_gotInterfaceWord = g_gotInterfaceWord || (msg.find("interface") != std::string::npos);
    g_gotChecksumWord = g_gotChecksumWord || (msg.find("checksum") != std::string::npos);
}

#define TEST_LOGS_ERROR(...) \
    do { \
        g_errorsReported = false; \
        g_gotInterfaceWord = false; \
        g_gotChecksumWord = false; \
        JOINT_CALL(Joint_SetLogCallback(&ChecksumTestsLogCallback)); \
        __VA_ARGS__; \
        JOINT_CALL(Joint_SetLogCallback(nullptr)); \
        TEST_EQUALS(g_errorsReported, true); \
        if (!g_gotInterfaceWord || !g_gotChecksumWord) \
            TEST_REPORT_WARNING("It is better if you have the words 'interface' and 'checksum' in the logged error message"); \
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
    TEST_LOGS_ERROR(TEST_THROWS_ANYTHING(t->AcceptCS2(c2)));
    TEST_LOGS_ERROR(TEST_THROWS_ANYTHING(t->CastToCS2(c12)));
}

#endif
