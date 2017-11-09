#ifndef TEST_TESTS_BASICTESTS_HPP
#define TEST_TESTS_BASICTESTS_HPP


class BasicTestsCallback
{
public:
    typedef joint::TypeList<
            test::IBasicTestsCallbackU8,
            test::IBasicTestsCallbackI8,
            test::IBasicTestsCallbackU16,
            test::IBasicTestsCallbackI16,
            test::IBasicTestsCallbackU32,
            test::IBasicTestsCallbackI32,
            test::IBasicTestsCallbackU64,
            test::IBasicTestsCallbackI64,
            test::IBasicTestsCallbackF32,
            test::IBasicTestsCallbackF64,
            test::IBasicTestsCallbackBool,
            test::IBasicTestsCallbackString
        > JointInterfaces;

    uint8_t   AddU8(uint8_t l, uint8_t r)     { return l + r; }
    int8_t    AddI8(int8_t l, int8_t r)       { return l + r; }
    uint16_t  AddU16(uint16_t l, uint16_t r)  { return l + r; }
    int16_t   AddI16(int16_t l, int16_t r)    { return l + r; }
    uint32_t  AddU32(uint32_t l, uint32_t r)  { return l + r; }
    int32_t   AddI32(int32_t l, int32_t r)    { return l + r; }
    uint64_t  AddU64(uint64_t l, uint64_t r)  { return l + r; }
    int64_t   AddI64(int64_t l, int64_t r)    { return l + r; }

    float     AddF32(float l, float r)        { return l + r; }
    double    AddF64(double l, double r)      { return l + r; }

    bool And(bool l, bool r) { return l && r; }

    joint::String Concat(joint::StringRef l, joint::StringRef r)
    { return l + r; }
};

TEST_DEFINE_TEST(TestCtx, BasicTests)
{
    auto t = Module.GetRootObject<test::IBasicTests>("GetTests");
    if (!t)
    {
        TEST_REPORT_ERROR("BasicTests not implemented!");
        return;
    }

    TEST_EQUALS(t->AddI8(2, 12), (int8_t)14);
    TEST_EQUALS(t->AddU8(2, 12), (uint8_t)14);
    TEST_EQUALS(t->AddI16(2, 12), (int16_t)14);
    TEST_EQUALS(t->AddU16(2, 12), (uint16_t)14);
    TEST_EQUALS(t->AddI32(2, 12), (int32_t)14);
    TEST_EQUALS(t->AddU32(2, 12), (uint32_t)14);
    TEST_EQUALS(t->AddI64(2, 12), (int64_t)14);
    TEST_EQUALS(t->AddU64(2, 12), (uint64_t)14);
    TEST_EQUALS(t->AddF32(2.1f, 12.2f), 14.3f);
    TEST_EQUALS(t->AddF64(2.1, 12.2), 14.3);

    TEST_EQUALS(t->And(true, true), true);
    TEST_EQUALS(t->And(true, false), false);

    TEST_EQUALS(t->Concat("abc", "xyz"), joint::String("abcxyz"));

    auto cb = Ctx.MakeComponent<test::IBasicTestsCallbackU8, BasicTestsCallback>();
    TEST_EQUALS(t->CallbackI8(joint::Cast<test::IBasicTestsCallbackI8>(cb), 2, 12), (int8_t)14);
    TEST_EQUALS(t->CallbackU8(joint::Cast<test::IBasicTestsCallbackU8>(cb), 2, 12), (uint8_t)14);
    TEST_EQUALS(t->CallbackI16(joint::Cast<test::IBasicTestsCallbackI16>(cb), 2, 12), (int16_t)14);
    TEST_EQUALS(t->CallbackU16(joint::Cast<test::IBasicTestsCallbackU16>(cb), 2, 12), (uint16_t)14);
    TEST_EQUALS(t->CallbackI32(joint::Cast<test::IBasicTestsCallbackI32>(cb), 2, 12), (int32_t)14);
    TEST_EQUALS(t->CallbackU32(joint::Cast<test::IBasicTestsCallbackU32>(cb), 2, 12), (uint32_t)14);
    TEST_EQUALS(t->CallbackI64(joint::Cast<test::IBasicTestsCallbackI64>(cb), 2, 12), (int64_t)14);
    TEST_EQUALS(t->CallbackU64(joint::Cast<test::IBasicTestsCallbackU64>(cb), 2, 12), (uint64_t)14);
    TEST_EQUALS(t->CallbackF32(joint::Cast<test::IBasicTestsCallbackF32>(cb), 2.1f, 12.2f), 14.3f);
    TEST_EQUALS(t->CallbackF64(joint::Cast<test::IBasicTestsCallbackF64>(cb), 2.1, 12.2), 14.3);

    TEST_EQUALS(t->CallbackBool(joint::Cast<test::IBasicTestsCallbackBool>(cb), true, true), true);
    TEST_EQUALS(t->CallbackBool(joint::Cast<test::IBasicTestsCallbackBool>(cb), true, false), false);

    TEST_EQUALS(t->CallbackString(joint::Cast<test::IBasicTestsCallbackString>(cb), "abc", "xyz"), joint::String("abcxyz"));
}

#endif
