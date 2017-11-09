#ifndef TEST_TESTS_OBJECTTESTS_HPP
#define TEST_TESTS_OBJECTTESTS_HPP


class SomeObject
{
private:
    int _counter{0};

public:
    using JointInterfaces = joint::TypeList<test::ISomeObject>;

    void Method() { ++_counter; }
    int32_t GetInvokationsCount() { return _counter; }
};

class ObjectTestsCallback
{
private:
    test::ISomeObject_Ptr _obj;

public:
    typedef joint::TypeList<
            test::IObjectTestsCallbackReturn,
            test::IObjectTestsCallbackParam
        > JointInterfaces;

    ObjectTestsCallback(const test::ISomeObject_Ptr& obj)
        : _obj(obj)
    { }

    test::ISomeObject_Ptr Return()
    { return _obj; }

    bool Method(test::ISomeObject_Ref o)
    { return (bool)o; }
};

TEST_DEFINE_TEST(TestCtx, ObjectTests)
{
    auto t = Module.GetRootObject<test::IObjectTests>("GetTests");
    if (!t)
    {
        TEST_REPORT_ERROR("ObjectTests not implemented!");
        return;
    }

    auto some_obj = t->ReturnNewObject();
    TEST_THROWS_NOTHING(some_obj->Method());

    TEST_EQUALS((bool)t->ReturnNull(), false);
    TEST_EQUALS(t->CheckNotNull(nullptr), false);
    TEST_EQUALS(t->CheckNotNull(some_obj), true);

    TEST_EQUALS((bool)t->CallbackReturn(Ctx.MakeComponent<test::IObjectTestsCallbackReturn, ObjectTestsCallback>(some_obj)), true);
    auto cb = Ctx.MakeComponent<test::IObjectTestsCallbackReturn, ObjectTestsCallback>(nullptr);
    TEST_EQUALS((bool)t->CallbackReturn(cb), false);
    TEST_EQUALS(t->CallbackParam(joint::Cast<test::IObjectTestsCallbackParam>(cb), nullptr), false);
    TEST_EQUALS(t->CallbackParam(joint::Cast<test::IObjectTestsCallbackParam>(cb), some_obj), true);

    auto o_impl = Ctx.MakeComponentWrapper<SomeObject>();
    auto o = Ctx.MakeComponentProxy<test::ISomeObject>(o_impl);

    TEST_THROWS_NOTHING(t->InvokeObjectMethod(o));
    TEST_EQUALS(o_impl->GetInvokationsCount(), 1);

    auto o2 = t->ReturnSameObject(o);
    o2->Method();
    TEST_EQUALS(o_impl->GetInvokationsCount(), 2);
}

#endif
