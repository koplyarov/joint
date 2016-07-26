#include <test/cpp/Tests.h>

#include <string.h>
#include <string>

#include <stdio.h>

#include <ShouldBeGenerated.hpp>
#include <joint.cpp/Component.hpp>


class Tests : public joint::Component<Tests, test::IBasicTests_impl, test::IStringTests_impl>
{
public:
	~Tests()
	{
		printf("CPP: dtor\n");
	}

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
};

extern "C"
{

	joint::IJointObject* GetBasicTests()
	{ return joint::MakeComponent<Tests>(); }

}
