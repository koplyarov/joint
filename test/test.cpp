#include <iostream>
#include <map>
#include <memory>

#include <Tests_adapters.hpp>
#include <test/core/Tests.hpp>


std::string g_bindingName = "cpp";
std::string g_moduleName = "tests-c";

struct TestCtx
{
	joint::Context			Ctx;
	joint::Module			Module;

	TestCtx()
		: Module(Ctx.LoadModule(g_bindingName, g_moduleName))
	{ }
};


TEST_DEFINE_TEST(TestCtx, BasicTests)
{
	auto t = Module.GetRootObject<test::IBasicTests>("GetTests");

	TEST_THROWS_ANYTHING(t->Throw());

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

	TEST_EQUALS(t->Concat("abc", "xyz"), std::string("abcxyz"));

	TEST_EQUALS(t->And(true, true), true);
	TEST_EQUALS(t->And(true, false), false);
}

#if 0
class SomeObject
{
private:
	mutable std::mutex	_mutex;
	int					_methodInvokationCounter{0};

public:
	typedef joint::TypeList<test::ISomeObject>	JointInterfaces;

	void Method() { std::lock_guard<std::mutex> l(_mutex); ++_methodInvokationCounter; }
	int GetMethodInvokationCounter() const { std::lock_guard<std::mutex> l(_mutex); return _methodInvokationCounter; }
};

TEST_DEFINE_TEST(TestCtx, ObjectTests)
{
	auto t = Module.GetRootObject<test::IObjectTests>("GetTests");

	auto so = joint::MakeComponentWrapper<SomeObject>();
	TEST_THROWS_NOTHING(t->InvokeObjectMethod(Ctx.MakeComponentProxy<test::ISomeObject>(so)));
	TEST_EQUALS(so->GetMethodInvokationCounter(), 1);
}


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

	auto listenable = t->CreateListenable();
	auto listener = joint::MakeComponentWrapper<LifetimeListener>();
	TEST_THROWS_NOTHING(listenable->SetListener(Ctx.MakeComponentProxy<test::ILifetimeListener>(listener)));
	TEST_EQUALS(listener->GetObjectDestroyed(), false);
	listenable.Reset();
	t->CollectGarbage();
	TEST_EQUALS(listener->GetObjectDestroyed(), true);
}
#endif


int main()
{
	try
	{ test::RunAllTests(); }
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
