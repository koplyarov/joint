#ifndef TEST_TESTS_ENUMTESTS_HPP
#define TEST_TESTS_ENUMTESTS_HPP

class EnumTestsCallback
{
public:
    typedef joint::TypeList<test::IEnumTests> JointInterfaces;

    joint::String StringRepresentation(test::Enum e) { return e.ToString(); }
    test::Enum GetNextValueInRing(test::Enum e)
    {
        switch(e)
        {
            case test::Enum::Value1: return test::Enum::Value2;
            case test::Enum::Value2: return test::Enum::Value3;
            case test::Enum::Value3: return test::Enum::Value4;
            case test::Enum::Value4: return test::Enum::Value1;
            default: JOINT_CPP_THROW(joint::Exception("Unknown enum value!"));
        }
    }
};

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
    TEST_EQUALS(t->GetNextValueInRing(test::Enum::Value3)._RawValue(), test::Enum::Value4);
    TEST_EQUALS(t->GetNextValueInRing(test::Enum::Value4)._RawValue(), test::Enum::Value1);


    e = test::Enum::Value3;
    auto tcb = Module.GetRootObject<test::IEnumCallbackTests>("GetTests");
    auto cb = Ctx.MakeComponent<joint::IObject, EnumTestsCallback>();
    TEST_EQUALS(tcb->CallbackStringRepresentation(joint::Cast<test::IEnumTests>(cb), e), joint::String("Value3"));
    TEST_EQUALS(tcb->CallbackGetNextValueInRing(joint::Cast<test::IEnumTests>(cb), test::Enum::Value1)._RawValue(), test::Enum::Value2);
    TEST_EQUALS(tcb->CallbackGetNextValueInRing(joint::Cast<test::IEnumTests>(cb), test::Enum::Value2)._RawValue(), test::Enum::Value3);
    TEST_EQUALS(tcb->CallbackGetNextValueInRing(joint::Cast<test::IEnumTests>(cb), test::Enum::Value3)._RawValue(), test::Enum::Value4);
    TEST_EQUALS(tcb->CallbackGetNextValueInRing(joint::Cast<test::IEnumTests>(cb), test::Enum::Value4)._RawValue(), test::Enum::Value1);
}

#endif
