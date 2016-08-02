#include <test/core/TestContext.hpp>

#include <iostream>


namespace test
{

	TestContext::TestContext(TestEngine* engine, std::string testName, const Location& location)
		: _engine(engine), _testName(std::move(testName)), _location(std::move(location)), _failed(false)
	{ }

	TestContext::~TestContext()
	{
		if (_failed)
			_engine->GetListener()->TestFailed(_testName, _location);
		else
			_engine->GetListener()->TestSucceeded(_testName, _location);
	}

	void TestContext::AssertionFailed(const Location& location, const std::string& message)
	{
		_engine->GetListener()->AssertionFailed(_testName, location, message);
		_failed = true;
	}

}
