#include <test/core/TestEngine.hpp>

#include <mutex>
#include <stdexcept>

#include <test/core/utils/Config.hpp>


#if TEST_PLATFORM_POSIX
#	include <unistd.h>
#endif


namespace test
{

	class DefaultTestResultsListener : public ITestResultsListener
	{
		enum Attr { ResetAll  = 0, Bright  = 1, Dim  = 2, Underline  = 3, Blink  = 5, Reverse = 7, Hidden = 8 };
		enum Color { Black = 0, Red = 1, Green = 2, Yellow = 3, Blue = 4, Magenta = 5, Cyan = 6, White = 7 };

	private:
		std::mutex		_mutex;
		int				_succeeded = 0;
		int				_failed = 0;
		int				_warnings = 0;
		bool			_hasTests = false;

	public:
		~DefaultTestResultsListener()
		{
			if (!_hasTests)
				return;

			std::string color = EscapeColor(Bright, (_failed == 0) ? (_warnings == 0 ? Green : Yellow) : Red);
			printf("%s", color.c_str());
			printf("%d tests succeeded", _succeeded);
			if (_failed > 0)
				printf(", %d failed", _failed);
			if (_warnings > 0)
				printf(", %d warnings", _warnings);
			printf("%s\n", ResetColor().c_str());
		}

		virtual void RunningTest(const std::string& testName, const Location& location)
		{
			std::lock_guard<std::mutex> l(_mutex);
			_hasTests = true;
		}

		virtual void TestSucceeded(const std::string& testName, const Location& location)
		{
			std::lock_guard<std::mutex> l(_mutex);
			//printf("%s%s succeeded%s\n", EscapeColor(Bright, Green).c_str(), testName.c_str(), ResetColor().c_str());
			++_succeeded;
		}

		virtual void TestFailed(const std::string& testName, const Location& location)
		{
			std::lock_guard<std::mutex> l(_mutex);
			printf("%s%s: %s failed%s\n",
				EscapeColor(Bright, Red).c_str(),
				location.ToString().c_str(),
				testName.c_str(),
				ResetColor().c_str());
			++_failed;
		}

		virtual void ReportWarning(const std::string& testName, const Location& location, const std::string& message)
		{
			std::lock_guard<std::mutex> l(_mutex);
			printf("%s%s: Warning in test %s: \n%s%s\n",
				EscapeColor(Bright, Yellow).c_str(),
				location.ToString().c_str(),
				testName.c_str(),
				message.c_str(),
				ResetColor().c_str());
			++_warnings;
		}

		virtual void AssertionFailed(const std::string& testName, const Location& location, const std::string& message)
		{
			printf("%s%s: Assertion in test %s failed: \n%s%s\n",
				EscapeColor(ResetAll, Yellow).c_str(),
				location.ToString().c_str(),
				testName.c_str(),
				message.c_str(),
				ResetColor().c_str());
		}

	private:
		static std::string ResetColor()
		{
#if TEST_PLATFORM_POSIX
			if (isatty(1))
				return "\33[0m";
			else
#endif
				return "";
		}

		static std::string EscapeColor(Attr attr, Color color)
		{
#if TEST_PLATFORM_POSIX
			if (isatty(1))
				return "\33[" + std::to_string((int)attr) + ";3" + std::to_string((int)color) + "m";
			else
#endif
				return "";
		}
	};


	TestEngine::TestEngine()
		: _resultsListener(new DefaultTestResultsListener)
	{ }


}
