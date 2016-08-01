#include <test/core/Tests.hpp>

#include <iostream>


namespace test
{
	namespace detail
	{
		void TestsContext::RegisterTest(std::string name, std::function<void()> f)
		{ _tests.insert({name, f}); }

		void TestsContext::RunAllTests()
		{
			for (auto& test : _tests)
			{
				std::cout << test.first << std::endl;
				test.second();
			}
		}

		TestsContext& TestsContext::GetInstance()
		{
			static TestsContext inst;
			return inst;
		}
	}

}
