#ifndef TEST_TESTS_EXCEPTIONTESTS_HPP
#define TEST_TESTS_EXCEPTIONTESTS_HPP


class ExceptionTestsCallback
{
private:
	std::function<void()>	_f;

public:
	typedef joint::TypeList<test::IExceptionTestsCallback> JointInterfaces;

	ExceptionTestsCallback(std::function<void()> f) : _f(std::move(f)) { }

	void Method() { _f(); }
};

TEST_DEFINE_TEST(TestCtx, ExceptionTests)
{
	auto t = Module.GetRootObject<test::IExceptionTests>("GetTests");
	if (!t)
	{
		TEST_REPORT_ERROR("ExceptionTests not implemented!");
		return;
	}

	TEST_THROWS(joint::Exception, t->ThrowNative());

	auto throw_f = [] { throw std::runtime_error("Test exception"); };
	auto throw_cb = Ctx.MakeComponent<test::IExceptionTestsCallback, ExceptionTestsCallback>(throw_f);
	TEST_EQUALS(t->CatchAll(throw_cb), true);
	TEST_THROWS(joint::Exception, t->LetThrough(throw_cb));

	auto nothrow_f = [] { };
	auto nothrow_cb = Ctx.MakeComponent<test::IExceptionTestsCallback, ExceptionTestsCallback>(nothrow_f);
	TEST_EQUALS(t->CatchAll(nothrow_cb), false);
	TEST_THROWS_NOTHING(t->LetThrough(nothrow_cb));
}

#endif
