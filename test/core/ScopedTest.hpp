#ifndef TEST_CORE_SCOPEDTEST_HPP
#define TEST_CORE_SCOPEDTEST_HPP


#include <string>

#include <test/core/TestContext.hpp>


namespace test
{

	class ScopedTest
	{
	private:
		thread_local static TestContext*	s_currentContext;

		TestContext&						_context;
		TestContext*						_prevContext;

	public:
		ScopedTest(TestContext& ctx, std::string name);
		~ScopedTest();

		static TestContext& GetCurrentContext();
	};

}

#endif
