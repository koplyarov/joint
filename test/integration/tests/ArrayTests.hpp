#ifndef TEST_TESTS_ARRAYTESTS_HPP
#define TEST_TESTS_ARRAYTESTS_HPP


TEST_DEFINE_TEST(TestCtx, ArrayTests)
{
    auto t = Module.GetRootObject<test::IArrayTests>("GetTests");
    if (!t)
    {
        TEST_REPORT_ERROR("ArrayTests not implemented!");
        return;
    }

    joint::Array<int32_t> i32_array = t->MakeI32Array(2);
    TEST_EQUALS(i32_array.GetSize(), (size_t)2);
    TEST_EQUALS(i32_array[0], (int32_t)0);
    TEST_THROWS_NOTHING(i32_array.Set(0, 42));
    TEST_THROWS_NOTHING(i32_array.Set(1, 256));
    TEST_EQUALS(i32_array[0], (int32_t)42);
    TEST_EQUALS(i32_array[1], (int32_t)256);

    i32_array = joint::Array<int32_t>(4);
    i32_array.Set(0, 1);
    i32_array.Set(1, 2);
    i32_array.Set(2, 4);
    i32_array.Set(3, 8);
    TEST_EQUALS(t->GetI32Element(i32_array, 0), (int32_t)1);
    TEST_EQUALS(t->GetI32Element(i32_array, 1), (int32_t)2);
    TEST_EQUALS(t->GetI32Element(i32_array, 2), (int32_t)4);
    TEST_EQUALS(t->GetI32Element(i32_array, 3), (int32_t)8);

    t->SetI32Element(i32_array, 0, (int32_t)2);
    t->SetI32Element(i32_array, 1, (int32_t)3);
    t->SetI32Element(i32_array, 2, (int32_t)5);
    t->SetI32Element(i32_array, 3, (int32_t)9);

    TEST_EQUALS(t->GetI32Element(i32_array, 0), (int32_t)2);
    TEST_EQUALS(t->GetI32Element(i32_array, 1), (int32_t)3);
    TEST_EQUALS(t->GetI32Element(i32_array, 2), (int32_t)5);
    TEST_EQUALS(t->GetI32Element(i32_array, 3), (int32_t)9);
}

#endif
