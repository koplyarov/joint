#include <test/core/TestContext.hpp>


namespace test
{

	class DefaultTestResultsListener : public ITestResultsListener
	{
	public:
		virtual void TestFailed(const std::string& message)
		{ printf("TEST FAILED: %s\n", message.c_str()); }
	};


	TestContext::TestContext()
		: _resultsListener(new DefaultTestResultsListener)
	{ }

}
