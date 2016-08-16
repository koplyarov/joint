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

	std::string Concat(const std::string& l, const std::string& r)
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

	TEST_EQUALS(t->Concat("abc", "xyz"), std::string("abcxyz"));

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

	TEST_EQUALS(t->CallbackString(joint::Cast<test::IBasicTestsCallbackString>(cb), "abc", "xyz"), std::string("abcxyz"));
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

class ObjectTestsCallback
{
private:
	test::ISomeObject_Ptr	_obj;

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

	bool Method(test::ISomeObject_Ptr o)
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


TEST_DEFINE_TEST(TestCtx, ExceptionTests)
{
	auto t = Module.GetRootObject<test::IExceptionTests>("GetTests");
	if (!t)
	{
		TEST_REPORT_ERROR("ExceptionTests not implemented!");
		return;
	}

	try
	{ t->ThrowNative(); }
	catch (const std::exception& ex)
	{ Joint_Log(JOINT_LOGLEVEL_WARNING, "Test", "%s", ex.what()); }

	TEST_THROWS(joint::detail::JointCppException, t->ThrowNative());
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
