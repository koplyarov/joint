#include <test/cpp/Tests.h>

#include <test/Tests_adapters.hpp>


class SomeObject
{
public:
	typedef joint::TypeList<test::ISomeObject>	JointInterfaces;

	void Method()
	{ }
};


class LifetimeListenable
{
private:
	test::ILifetimeListener_Ptr	_listener;

public:
	typedef joint::TypeList<test::ILifetimeListenable>	JointInterfaces;

	~LifetimeListenable()
	{
		if (_listener)
			_listener->OnDestroyed();
	}

	void SetListener(const test::ILifetimeListener_Ptr& l)
	{ _listener = l; }
};


class Tests
{
public:
	typedef joint::TypeList<test::IBasicTests, test::IObjectTests, test::ILifetimeTests>	JointInterfaces;

private:
	Joint_ModuleHandle   _module;

public:
	Tests(Joint_ModuleHandle module)
		: _module(module)
	{ }

	~Tests()
	{ }

	void Throw() { }

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

	void InvokeObjectMethod(const test::ISomeObject_Ptr& o)
	{ o->Method(); }

	test::ISomeObject_Ptr ReturnSameObject(const test::ISomeObject_Ptr& o)
	{ return o; }

	test::ISomeObject_Ptr ReturnNewObject()
	{ return joint::MakeComponent<test::ISomeObject, SomeObject>(_module); }


	test::ILifetimeListenable_Ptr CreateListenable()
	{ return joint::MakeComponent<test::ILifetimeListenable, LifetimeListenable>(_module); }

	void CollectGarbage()
	{ }
};

extern "C"
{

	Joint_ObjectHandle GetTests(Joint_ModuleHandle module)
	{ return joint::Export(joint::MakeComponent<joint::IObject, Tests>(module, module)); }

}
