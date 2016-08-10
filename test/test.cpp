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

	TEST_EQUALS(t->Concat("abc", "xyz"), std::string("abcxyz"));

	TEST_EQUALS(t->And(true, true), true);
	TEST_EQUALS(t->And(true, false), false);
}


class SomeObject
{
private:
	int					_counter{0};

public:
	typedef joint::TypeList<test::ISomeObject>	JointInterfaces;

	void Method() { ++_counter; }
	int32_t GetInvokationsCount() { return _counter; }
};

class NullChecksCallback
{
private:
	joint::Context		_ctx;

public:
	typedef joint::TypeList<test::INullChecksCallback>	JointInterfaces;

	bool				ReturnNullInvoked = false;
	bool				ReturnNotNullInvoked = false;
	bool				ValidateNullInvoked = false;
	bool				ValidateNotNullInvoked = false;

	NullChecksCallback(joint::Context ctx)
		: _ctx(std::move(ctx))
	{ }

	test::ISomeObject_Ptr ReturnNull()
	{ ReturnNullInvoked = true; return test::ISomeObject_Ptr(); }

	test::ISomeObject_Ptr ReturnNotNull()
	{ ReturnNotNullInvoked = true; return _ctx.MakeComponent<test::ISomeObject, SomeObject>(); }

	bool ValidateNotNull(test::ISomeObject_Ptr o, bool notNull)
	{
		if (notNull)
			ValidateNullInvoked = true;
		else
			ValidateNotNullInvoked = true;
		return notNull == (bool)o;
	}
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
	TEST_EQUALS(t->CheckNotNull(test::ISomeObject_Ptr()), false);
	TEST_EQUALS(t->CheckNotNull(some_obj), true);

	auto cb_impl = joint::MakeComponentWrapper<NullChecksCallback>(Ctx);
	auto cb = Ctx.MakeComponentProxy<test::INullChecksCallback>(cb_impl);
	TEST_EQUALS(t->ReverseNullChecks(cb), true);
	TEST_EQUALS(cb_impl->ReturnNullInvoked, true);
	TEST_EQUALS(cb_impl->ReturnNotNullInvoked, true);
	TEST_EQUALS(cb_impl->ValidateNullInvoked, true);
	TEST_EQUALS(cb_impl->ValidateNotNullInvoked, true);

	auto o_impl = joint::MakeComponentWrapper<SomeObject>();
	auto o = Ctx.MakeComponentProxy<test::ISomeObject>(o_impl);

	TEST_THROWS_NOTHING(t->InvokeObjectMethod(o));
	TEST_EQUALS(o_impl->GetInvokationsCount(), 1);

	auto o2 = t->ReturnSameObject(o);
	o2->Method();
	TEST_EQUALS(o_impl->GetInvokationsCount(), 2);
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


int main(int argc, const char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <binding> <module>" << std::endl;
		return 1;
	}
	g_bindingName = argv[1];
	g_moduleName = argv[2];
	std::cerr << "Binding: " << g_bindingName << ", module: " << g_moduleName << std::endl;

	try
	{ test::RunAllTests(); }
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
