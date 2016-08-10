#ifndef TEST_CORE_TESTCONTEXT_HPP
#define TEST_CORE_TESTCONTEXT_HPP


#include <mutex>
#include <string>

#include <test/core/Location.hpp>
#include <test/core/TestEngine.hpp>


namespace test
{

	class TestContext
	{
	private:
		TestEngine*		_engine;
		std::string		_testName;
		Location		_location;
		bool			_failed;

	public:
		TestContext(TestEngine* engine, std::string testName, const Location& location);
		~TestContext();

		void AssertionFailed(const Location& location, const std::string& message);
		void ReportWarning(const Location& location, const std::string& message);
	};

}

#endif
