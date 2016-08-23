#ifndef TEST_TESTS_LIFETIMETESTS_HPP
#define TEST_TESTS_LIFETIMETESTS_HPP


class LifetimeListener
{
private:
	mutable std::mutex	_mutex;
	bool				_objectDestroyed{0};

public:
	typedef joint::TypeList<test::ILifetimeListener>	JointInterfaces;

	void OnDestroyed() { std::lock_guard<std::mutex> l(_mutex); _objectDestroyed = true; }
	bool GetObjectDestroyed() const { std::lock_guard<std::mutex> l(_mutex); return _objectDestroyed; }
};


TEST_DEFINE_TEST(TestCtx, LifetimeTests)
{
	auto t = Module.GetRootObject<test::ILifetimeTests>("GetTests");
	if (!t)
	{
		TEST_REPORT_ERROR("LifetimeTests not implemented!");
		return;
	}

	auto listenable = t->CreateListenable();
	auto listener = joint::MakeComponentWrapper<LifetimeListener>();
	TEST_THROWS_NOTHING(listenable->SetListener(Ctx.MakeComponentProxy<test::ILifetimeListener>(listener)));
	TEST_EQUALS(listener->GetObjectDestroyed(), false);
	listenable.Reset();
	t->CollectGarbage();
	TEST_EQUALS(listener->GetObjectDestroyed(), true);
}

#endif
