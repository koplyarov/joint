#include <test/cpp/Tests.h>

#include <string.h>
#include <string>

#include <stdio.h>

#include <test/Tests_adapters.hpp>
#include <ShouldBeGenerated.hpp>
#include <joint.cpp/Component.hpp>


class SomeObject : public joint::Component<test::ISomeObject_impl>
{
public:
	SomeObject(Joint_ModuleHandle module)
	{ printf("CPP: SomeObject ctor\n"); }

	~SomeObject()
	{ printf("CPP: SomeObject dtor\n"); }

	virtual void Method()
	{ printf("CPP: Method\n"); }
};


class Tests : public joint::Component<test::IBasicTests_impl, test::IStringTests_impl, test::IObjectTests_impl>
{
private:
	Joint_ModuleHandle   _module;

public:
	Tests(Joint_ModuleHandle module)
		: _module(module)
	{ printf("CPP: Tests ctor\n"); }

	~Tests()
	{ printf("CPP: Tests dtor\n"); }

	virtual uint32_t AddU32(uint32_t l, uint32_t r)
	{
		printf("CPP: AddU32\n");
		return l + r;
	}

	virtual int32_t AddI32(int32_t l, int32_t r)
	{
		printf("CPP: AddI32\n");
		return l + r;
	}

	virtual std::string Concat(const std::string& l, const std::string& r)
	{
		printf("CPP: Concat\n");
		return l + r;
	}

	virtual void InvokeObjectMethod(const test::ISomeObject_Ptr& o)
	{
		printf("CPP: InvokeObjectMethod\n");
		o->Method();
	}

	virtual test::ISomeObject_Ptr ReturnNewObject()
	{ return joint::MakeComponent<test::ISomeObject, SomeObject>(_module, _module); }
};

extern "C"
{

	joint::IObject* GetBasicTests(Joint_ModuleHandle module)
	{ return joint::MakeComponent<joint::IObject, Tests>(module, module).NewRef(); }

	joint::IObject* GetSomeObject(Joint_ModuleHandle module)
	{ return joint::MakeComponent<joint::IObject, SomeObject>(module, module).NewRef(); }

}
