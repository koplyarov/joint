#ifndef TEST_TESTS_CASTTESTS_HPP
#define TEST_TESTS_CASTTESTS_HPP


class CastObject
{
public:
    typedef joint::TypeList<
            test::IInterface0,
            test::IInterface1,
            test::IInterface7
        > JointInterfaces;

    int8_t Method1(int8_t i) { return i; }
    int16_t Method3(int16_t i) { return i; }
    uint16_t Method4(uint16_t i) { return i; }
    int32_t Method5(int32_t i) { return i; }
    uint32_t Method6(uint32_t i) { return i; }
    int64_t Method7(int64_t i) { return i; }
};

TEST_DEFINE_TEST(TestCtx, CastTests)
{
    auto t = Module.GetRootObject<test::ICastTests>("GetTests");
    if (!t)
    {
        TEST_REPORT_ERROR("CastTests not implemented!");
        return;
    }

    {
        TEST_EQUALS((bool)t->CastTo2(nullptr), false);

        auto o0 = Ctx.MakeComponent<test::IInterface0, CastObject>();
        auto o1 = t->CastTo1(o0);
        auto o2 = t->CastTo2(o0);
        auto o3 = t->CastTo3(o0);
        auto o4 = t->CastTo4(o0);
        auto o5 = t->CastTo5(o0);
        auto o6 = t->CastTo6(o0);
        auto o7 = t->CastTo7(o0);

        TEST_EQUALS((bool)o1, true);
        TEST_EQUALS((bool)o2, false);
        TEST_EQUALS((bool)o3, true);
        TEST_EQUALS((bool)o4, true);
        TEST_EQUALS((bool)o5, true);
        TEST_EQUALS((bool)o6, true);
        TEST_EQUALS((bool)o7, true);

        TEST_EQUALS(o1->Method1(10), (int8_t)10);
        TEST_EQUALS(o3->Method3(10), (int16_t)10);
        TEST_EQUALS(o4->Method4(10), (uint16_t)10);
        TEST_EQUALS(o5->Method5(10), (int32_t)10);
        TEST_EQUALS(o6->Method6(10), (uint32_t)10);
        TEST_EQUALS(o7->Method7(10), (int64_t)10);
    }

    {
        auto o0 = t->Create017();
        auto o1 = joint::Cast<test::IInterface1>(o0);
        auto o2 = joint::Cast<test::IInterface2>(o0);
        auto o3 = joint::Cast<test::IInterface3>(o0);
        auto o4 = joint::Cast<test::IInterface4>(o0);
        auto o5 = joint::Cast<test::IInterface5>(o0);
        auto o6 = joint::Cast<test::IInterface6>(o0);
        auto o7 = joint::Cast<test::IInterface7>(o0);

        TEST_EQUALS((bool)o0, true);
        TEST_EQUALS((bool)o1, true);
        TEST_EQUALS((bool)o2, false);
        TEST_EQUALS((bool)o3, true);
        TEST_EQUALS((bool)o4, true);
        TEST_EQUALS((bool)o5, true);
        TEST_EQUALS((bool)o6, true);
        TEST_EQUALS((bool)o7, true);

        TEST_THROWS_NOTHING(o1->Method1(10));
        TEST_THROWS_NOTHING(o3->Method3(10));
        TEST_THROWS_NOTHING(o4->Method4(10));
        TEST_THROWS_NOTHING(o5->Method5(10));
        TEST_THROWS_NOTHING(o6->Method6(10));
        TEST_THROWS_NOTHING(o7->Method7(10));
    }
}

#endif
