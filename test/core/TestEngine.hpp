#ifndef TEST_CORE_TESTENGINE_HPP
#define TEST_CORE_TESTENGINE_HPP


#include <memory>
#include <string>

#include <test/core/Location.hpp>


namespace test
{

	struct ITestResultsListener
	{
		virtual ~ITestResultsListener() { }

		virtual void TestSucceeded(const std::string& testName, const Location& location, const std::string& message) = 0;
		virtual void TestFailed(const std::string& testName, const Location& location, const std::string& message) = 0;
	};
	using ITestResultsListenerPtr = std::unique_ptr<ITestResultsListener>;


	class TestEngine
	{
	private:
		ITestResultsListenerPtr		_resultsListener;

	public:
		TestEngine();

		const ITestResultsListenerPtr& GetListener() const { return _resultsListener; }
	};

}

#endif
