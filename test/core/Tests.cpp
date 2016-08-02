#include <test/core/Tests.hpp>

#include <iostream>


namespace test
{
	namespace detail
	{
		void TestsRegistry::RegisterTest(std::string name, std::function<void()> f)
		{ _tests.insert({name, f}); }

		void TestsRegistry::RunAllTests()
		{
			for (auto& test : _tests)
				test.second();
		}

		TestsRegistry& TestsRegistry::GetInstance()
		{
			static TestsRegistry inst;
			return inst;
		}
	}

}
