#include <test/core/TestContext.hpp>


namespace test
{

	void TestContext::TestSucceeded(const Location& location, const std::string& message)
	{
		_engine->GetListener()->TestSucceeded(_testName, location, message);
	}

	void TestContext::TestFailed(const Location& location, const std::string& message)
	{
		_engine->GetListener()->TestFailed(_testName, location, message);
	}

}
