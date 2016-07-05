#ifndef TEST_CORE_TESTCONTEXT_HPP
#define TEST_CORE_TESTCONTEXT_HPP


#include <string>

#include <test/core/TestEngine.hpp>


namespace test
{

	class TestContext
	{
	private:
		TestEngine*		_engine;
		std::string		_testName;

	public:
		TestContext(TestEngine* engine, std::string testName)
			: _engine(engine), _testName(std::move(testName))
		{ }

		void TestSucceeded(const Location& location, const std::string& message);
		void TestFailed(const Location& location, const std::string& message);
	};

}

#endif
