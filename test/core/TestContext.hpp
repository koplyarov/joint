#ifndef TEST_CORE_TESTCONTEXT_HPP
#define TEST_CORE_TESTCONTEXT_HPP


#include <memory>


namespace test
{

	struct ITestResultsListener
	{
		virtual ~ITestResultsListener() { }

		virtual void TestFailed(const std::string& message) = 0;
	};
	using ITestResultsListenerPtr = std::unique_ptr<ITestResultsListener>;


	class TestContext
	{
	private:
		ITestResultsListenerPtr		_resultsListener;

	public:
		TestContext();
	};

}

#endif
