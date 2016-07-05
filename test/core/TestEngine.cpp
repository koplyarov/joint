#include <test/core/TestEngine.hpp>

#include <mutex>
#include <stdexcept>


namespace test
{

	class DefaultTestResultsListener : public ITestResultsListener
	{
	private:
		std::mutex		_mutex;
		int				_succeeded = 0;
		int				_failed = 0;

	public:
		~DefaultTestResultsListener()
		{
			printf("%d tests succeeded, %d failed\n", _succeeded, _failed);
		}

		virtual void TestSucceeded(const std::string& testName, const Location& location, const std::string& message)
		{
			std::lock_guard<std::mutex> l(_mutex);
			++_succeeded;
		}

		virtual void TestFailed(const std::string& testName, const Location& location, const std::string& message)
		{
			printf("[ FAILED   ]: %s\n", message.c_str());
			std::lock_guard<std::mutex> l(_mutex);
			++_failed;
		}
	};


	TestEngine::TestEngine()
		: _resultsListener(new DefaultTestResultsListener)
	{ }


}
