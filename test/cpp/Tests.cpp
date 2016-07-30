#include <test/cpp/Tests.h>

#include <string.h>
#include <string>

#include <stdio.h>

#include <test/Tests_adapters.hpp>
#include <joint.cpp/Component.hpp>


class SomeObject
{
public:
	typedef joint::TypeList<test::ISomeObject>	JointInterfaces;

	SomeObject(Joint_ModuleHandle module)
	{ }

	~SomeObject()
	{ }

	void Method()
	{ printf("CPP: Method\n"); }
};


class Tests
{
public:
	typedef joint::TypeList<test::IBasicTests, test::IObjectTests>	JointInterfaces;

private:
	Joint_ModuleHandle   _module;

public:
	Tests(Joint_ModuleHandle module)
		: _module(module)
	{ }

	~Tests()
	{ }

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
	{ return joint::MakeComponent<test::ISomeObject, SomeObject>(_module, _module); }
};

extern "C"
{

	Joint_ObjectHandle GetBasicTests(Joint_ModuleHandle module)
	{ return joint::Export(joint::MakeComponent<joint::IObject, Tests>(module, module)); }

}
